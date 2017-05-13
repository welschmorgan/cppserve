/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_test.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/21 22:03:41 by mwelsch           #+#    #+#             */
//   Updated: 2017/05/13 15:18:08 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		UNIT_TEST_H
# define	UNIT_TEST_H

# include <string>
# include <functional>
# include <iostream>
# include <sstream>
# include <chrono>

# ifdef UNICODE
typedef std::wstring			String;
typedef std::wstringstream		StringStream;
# else
typedef std::string				String;
typedef std::stringstream		StringStream;
# endif

namespace				unit_test
{
	template<typename ret_t, typename... args_t>
	class				func
	{
	public:
		typedef func<ret_t, args_t...>				self_type;
		typedef std::function<ret_t(args_t...)>		func_type;
		typedef ret_t								return_type;
		typedef std::chrono::high_resolution_clock	clock_type;
		typedef std::chrono::time_point<clock_type>	time_point_type;
		typedef String								string_type;
		typedef StringStream						string_stream_type;

		string_type								name;
		string_type								desc;
		func_type								handler;
		time_point_type							start_time;
		time_point_type							end_time;
		string_stream_type						_error;

		func(const string_type &name_ = string_type(),
			 const string_type &desc_ = string_type())
			: name(name_)
			, desc(desc_)
			, handler()
			, start_time(clock_type::now())
			, end_time(clock_type::now())
			, _error()
			{}
		func(const self_type &rhs)
			: name(rhs.name)
			, desc(rhs.desc)
			, handler(rhs.handler)
			, start_time(rhs.start_time)
			, end_time(rhs.end_time)
			, _error(rhs._error)
			{}
		func(const string_type &name_,
			 const string_type &desc_,
			 func_type fn)
			: name(name_)
  			, desc(desc_)
			, handler(fn)
			, start_time(clock_type::now())
			, end_time(clock_type::now())
			, _error()
			{}
		virtual ~func() throw() {}

		self_type						&operator=(const self_type &rhs)
			{
				name = rhs.name;
				desc = rhs.desc;
				handler = rhs.handler;
				start_time = rhs.start_time;
				end_time = rhs.end_time;
				_error = rhs._error;
				return (*this);
			}

		int								operator()(args_t... args)
			{ return (run(args...)); }

		double							took_ms() const throw()
			{
				auto diff_time = end_time - start_time;
				double ms = std::chrono::duration_cast<std::chrono::microseconds>(diff_time).count();
				return (ms * 0.001);
			}
		string_stream_type				&error() throw()
			{
				return (_error);
			}
		int								run(args_t... args) throw (std::runtime_error)
			{
				std::stringstream		ss;
				std::streambuf			*old_cout;
				std::streambuf			*old_cerr;
				std::streambuf			*old_clog;
				string_type				output;
				string_type				indent(1, '\t');

				start_time = clock_type::now();
				std::cout << "\033[0;32m"
						  << "* "
						  << "\033[0mstarting test '"
						  << "\033[0;34m"
						  << name
						  << "\033[0m'";
				if (!desc.empty())
					std::cout << std::endl << "[" << desc << "]";
				std::cout << std::endl;
				try {
					old_cout = std::cout.rdbuf(ss.rdbuf());
					old_cerr = std::cerr.rdbuf(ss.rdbuf());
					old_clog = std::clog.rdbuf(ss.rdbuf());
					handler(args...);
					std::cout.rdbuf(old_cout);
					std::cerr.rdbuf(old_cerr);
					std::clog.rdbuf(old_clog);
					end_time = clock_type::now();
					output = ss.str();
					trim(output);
					if (!output.empty() && output.at(0) != '\t')
						output = indent + "| " + output;
					replace(output, "\n", "\n" + indent + "| ");
					std::cout << output;
					if (!output.empty()
						&& output.at(output.size() - 1) != '\n')
						std::cout << std::endl;
					std::cout << "\033[0;32m"
							  << "[OK] "
							  << "\033[0mdone in "
							  << "\033[0;34m"
							  << took_ms()
							  << "ms\033[0m"
							  << std::endl;
				} catch (std::exception &ex) {
					if (!_error.str().empty())
						_error << std::endl;
					_error << string_type(ex.what());
					end_time = clock_type::now();
					std::cout.rdbuf(old_cout);
					std::cerr.rdbuf(old_cerr);
					std::clog.rdbuf(old_clog);
					std::cerr << "\033[0;31m"
							  << "-> "
							  << "\033[0mfailed after "
							  << "\033[0;34m"
							  << took_ms()
							  << "ms\033[0m"
							  << std::endl
							  << "\033[0;31m"
							  << ex.what()
							  << "ms\033[0m"
							  << std::endl;
				}
				return (!_error.str().empty());
			}

		string_type						&trim(string_type &str,
											  const string_type &charset = string_type(" \t\r\v\f\n"))
			{
				while (!str.empty() && charset.find(str.at(0)) != std::string::npos)
					str.erase(str.begin());
				while (!str.empty() && charset.find(str.at(str.size() - 1)) != std::string::npos)
					str.erase(str.begin() + (str.size() - 1));
				return (str);
			}
		bool							replace(string_type& str,
												const string_type& from,
												const string_type& to) {
			size_t						pos;

			pos = str.find(from);
			if (pos == std::string::npos)
				return (false);
			while (pos != std::string::npos)
			{
				str.replace(pos, from.length(), to);
				pos = str.find(from, pos + from.length());
			}
			return (true);
		}

	};
};

#endif
