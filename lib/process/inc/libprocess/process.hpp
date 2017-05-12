// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   process.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 11:58:51 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/30 13:18:25 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#ifndef PROCESSUS_HPP
# define PROCESSUS_HPP

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <unistd.h>
# include <cstring>
# include <iostream>
# include <vector>
# include <sstream>

namespace proc
{
	template<typename string_t>
	class									basic_process
	{
	public:
		typedef basic_process<string_t>		self_type;
		typedef uint32_t					flag_type;
		typedef pid_t						pid_type;
		typedef int							status_type;
		typedef uint8_t						state_type;
		typedef int							pipe_type[2];
		typedef string_t					string_type;
		typedef typename string_type::value_type	char_type;
		typedef std::vector<string_type>			arg_list_type;
		typedef std::basic_stringstream<char_type>	stream_type;

		enum							states
		{
			ST_NONE,
			ST_STARTING,
			ST_STARTED,
			ST_STOPPING,
			ST_STOPPED
		};
		enum							flags
		{
			FT_NONE = 0,
			FT_PRINT = 1 << 1,
			FT_EXCEPT = 1 << 2,
			FT_NOENV = 1 << 3,
			FT_COUNT = 4
		};

		static const flag_type			default_flags = FT_EXCEPT | FT_PRINT;
		static const state_type			default_state = ST_NONE;

	private:
		basic_process(const self_type &rk) = delete;
		self_type	&operator=(const self_type &rk) = delete;

	protected:
		string_t						m_path;
		pid_type						m_pid;
		status_type						m_status;
		state_type						m_state;
		flag_type						m_flags;
		pipe_type						m_pipe;
		pipe_type						m_epipe;
		arg_list_type					m_args;
		stream_type						m_output;
		stream_type						m_errors;

	public:
		basic_process(const string_t &path = string_t(),
					  const flag_type flags = default_flags)
			: m_path(path)
			, m_pid(0)
			, m_status(0)
			, m_state(ST_NONE)
			, m_flags(flags)
			, m_pipe()
			, m_epipe()
			, m_args()
			, m_output()
			, m_errors()
			{
				if (!path.empty())
				{
					struct stat	buf;
					if (stat(path.c_str(), &buf) == -1)
					{
						if ((m_flags & FT_EXCEPT) != FT_NONE)
							throw std::runtime_error("stat: " + string_t(strerror(errno)) + ": " + path);
					}
				}
			}
		virtual ~basic_process() throw()
			{}

		self_type					&flags(flag_type f) throw()
			{
				m_flags = f;
				return (*this);
			}

		flag_type					flags() const throw()
			{ return (m_flags); }

		self_type					&pid(pid_type p) throw()
			{
				m_pid = p;
				return (*this);
			}

		pid_type					pid() const throw()
			{ return (m_pid); }

		const string_t				output() const throw()
			{ return (m_output.str()); }
		stream_type					&output() throw()
			{ return (m_output); }

		const string_t				errors() const throw()
			{ return (m_errors.str()); }
		stream_type					&errors() throw()
			{ return (m_errors); }

		self_type					&status(status_type p) throw()
			{
				m_status = p;
				return (*this);
			}

		status_type					status() const throw()
			{ return (m_status); }

		self_type					&state(state_type p) throw()
			{
				m_state = p;
				return (*this);
			}

		state_type					state() const throw()
			{ return (m_state); }
		string_t					arg(int i) const throw()
			{
				if (i >= m_args.size())
					throw std::range_error("undefined arg#" + std::to_string(i));
				return (m_args.at(i));
			}
		self_type					&arg(const string_t &val)
			{
				m_args.push_back(val);
				return (*this);
			}
		string_t					path() const throw()
			{ return (m_path); }
		self_type					&path(const string_t &p)
			{ m_path = p; return (*this); }

		arg_list_type				args() const throw()
			{ return (m_args); }
		self_type					args(const arg_list_type &args) throw()
			{
				m_args = args;
				return (*this);
			}

		void						execute(const arg_list_type &args_ = arg_list_type()) throw(std::runtime_error)
			{
				static char			buffer[1024] = { 0 };
				char				**args = NULL;
				string_t			sbuffer;

				m_state = ST_NONE;
				if (!args_.empty())
					m_args = args_;
				if (m_path.empty())
					throw std::runtime_error("missing executable name!");
				std::cout << "exec: " << m_path << std::endl;
				args = new char*[m_args.size() + 2];
				args[0] = strndup(m_path.c_str(), m_path.size());
				for (size_t i = 0; i < m_args.size(); i++)
				{
					args[i + 1] = strndup(m_args[i].c_str(),
										  m_args[i].size());
					std::cout << "\t" << m_args[i] << std::endl;
				}
				args[m_args.size()] = NULL;
				if (pipe(m_pipe) == -1)
					throw std::runtime_error(strerror(errno));
				if (pipe(m_epipe) == -1)
					throw std::runtime_error(strerror(errno));
				if ((m_pid = fork()) == -1)
					throw std::runtime_error(strerror(errno));
				if (m_pid != 0)
				{
					// parent
					close(m_pipe[1]);
					close(m_epipe[1]);
					m_state = ST_STARTED;
					while (waitpid(m_pid,
								   &m_status,
								   WNOHANG) <= 0)
						usleep(150);
					while (read(m_pipe[0],
								buffer,
								sizeof(buffer)))
					{
						sbuffer = buffer;
						trim(sbuffer);
						if (!sbuffer.empty())
						{
							if (!m_output.str().empty())
								m_output << std::endl;
							m_output << sbuffer << std::endl;
						}
					}
					close(m_pipe[0]);
					while (read(m_epipe[0],
								buffer,
								sizeof(buffer)))
					{
						sbuffer = buffer;
						trim(sbuffer);
						if (!sbuffer.empty())
						{
							if (!m_errors.str().empty())
								m_errors << std::endl;
							m_errors << sbuffer;
						}
					}
					close(m_epipe[0]);
				}
				else
				{
					// child
					close(m_pipe[0]);
					close(m_epipe[0]);
					dup2(m_pipe[1], 1);
					dup2(m_epipe[1], 2);
					close(m_pipe[1]);
					close(m_epipe[1]);
					m_state = ST_STARTING;
					if ((m_flags & FT_NOENV) != FT_NONE)
						execve(args[0], args, environ);
					else
						execve(args[0], args, NULL);
					std::cerr << "execve: " << strerror(errno) << ": " << m_path << std::endl;
					_exit(EXIT_FAILURE);
				}
				for (size_t i = 0; i < (m_args.size() + 1); i++)
				{
					if (args[i])
						delete args[i], args[i] = NULL;
				}
				delete []args, args = NULL;
				if ((m_flags & FT_EXCEPT) != FT_NONE)
				{
					if (!m_errors.str().empty())
						throw std::runtime_error(m_errors.str());
				}
				if ((m_flags & FT_PRINT) != FT_NONE)
				{
					if (!m_errors.str().empty())
						std::cerr << m_errors.str() << std::endl;
					if (!m_output.str().empty())
						std::cerr << m_output.str() << std::endl;
				}
			}

	protected:
		string_type					&trim(string_type &str,
										  const string_type &charset = string_type(" \n\t"))
			{
				size_t				pos;
				while (!str.empty()
					   && (pos = charset.find(str.at(0))) != std::string::npos)
					str.erase(str.begin() + 0);
				while (!str.empty()
					   && (pos = charset.find(str.at(str.size() - 1))) != std::string::npos)
					str.erase(str.begin() + (str.size() - 1));
				return (str);
			}
	};
# ifdef UNICODE
	typedef basic_process<std::wstring> process;
# else
	typedef basic_process<std::string>	process;
# endif
};

#endif
