// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.hpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/29 16:04:33 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/29 19:03:28 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef MAIN_HPP
# define MAIN_HPP

#define STATE_STARTING 0
#define STATE_CONNECTED 1
#define STATE_TIMEDOUT 2
#define STATE_UNKNOWN 3


#include <chrono>
#include <iostream>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
using Duration = Clock::duration;

# include "request.hpp"
# include "host.hpp"
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>


template<typename string_t,
		 typename stream_t>
class			launcher
{
public:
	typedef std::map<int, sighandler_t>	signal_handler_map;

protected:
	launcher(const launcher &) = delete;
	launcher &operator=(const launcher &) = delete;

protected:
	pid_t				m_pid;
	int					m_pipefd[2];
	const size_t		m_timeout_ms;
	TimePoint			m_starttime_ms;
	size_t				m_uptime_ms;
	int					m_state;
	Host				m_addr;
	string_t			m_path;
	int					m_status;
	signal_handler_map	m_signals;
	unsigned			m_pings;
	string_t			m_response;
	stream_t			&m_log;
	string_t			m_captured_output;

	static launcher		*g_inst;

protected:
	static string_t		find_exec() throw() {
		string_t		path;
		static char		cwd[1024] = { 0 };
		static const std::vector<string_t>	tries
		{
			"../../cppserve"
		};
		getcwd(cwd, 1024);
		for(auto cur_try : tries)
		{
			path = String(cwd) + "/" + cur_try;
			std::cout << "trying " << path << "..." << std::endl;
		}
		return (path);
	}

	void				install_signal_handler(int sigc, sighandler_t handler) {
		m_signals[sigc] = signal(sigc, handler);
	}

	sighandler_t		uninstall_signal_handler(int sigc) {
		return (signal(sigc, m_signals[sigc]));
	}

	static void			_sig_handler(int sig) {
		g_inst->kill(sig);
	}


public:
	launcher(stream_t &logstrm,
			 const string_t &addr = string_t("localhost"),
			 const uint16_t port = uint16_t(8080),
			 const size_t timeout = 5000)
		: m_pid(-1)
		, m_pipefd()
		, m_timeout_ms(timeout)
		, m_uptime_ms(0)
		, m_state(STATE_UNKNOWN)
		, m_addr(addr, port)
		, m_path(find_exec())
		, m_status(0)
		, m_signals()
		, m_pings(0)
		, m_log(logstrm)
		, m_captured_output()
		{
			g_inst = this;
			install_signal_handler(SIGTERM, _sig_handler);
			install_signal_handler(SIGINT, _sig_handler);
			install_signal_handler(SIGQUIT, _sig_handler);
			install_signal_handler(SIGKILL, _sig_handler);
			install_signal_handler(SIGCHLD, _sig_handler);
		}
	virtual ~launcher() throw() {
		uninstall_signal_handler(SIGTERM);
		uninstall_signal_handler(SIGINT);
		uninstall_signal_handler(SIGQUIT);
		uninstall_signal_handler(SIGKILL);
		uninstall_signal_handler(SIGCHLD);
	}

	template<typename on_tick_t,
			 typename... tick_args_t>
	void					start(on_tick_t on_tick,
								  tick_args_t... args) {
		pid_t				code;
		char				buffer[1024];
		pipe(m_pipefd);
		m_starttime_ms = Clock::now();
		if ((m_pid = fork()) == -1)
			throw std::runtime_error("failed to fork!");
		if (m_pid == 0)
		{ // child
			static char			verbose[] = "-vvv";
			static char	*const	args[] = {
				&m_path[0], verbose, NULL
			};
			static char	*const	env[] = {
				NULL
			};
			close(m_pipefd[0]);
			dup2(m_pipefd[1], 1);
			dup2(m_pipefd[1], 2);
			close(m_pipefd[1]);
			m_state = STATE_STARTING;
			m_log << "starting server: " << m_path << std::endl;
			execve(args[0], args, env);
			_exit(EXIT_FAILURE);
		}
		else
		{ // parent
			close(m_pipefd[1]);
			while ((code = waitpid(m_pid, &m_status, WNOHANG)) <= 0)
			{
				usleep(300);
				m_uptime_ms = uptime();
				on_tick(args...);
				usleep(300);
				if (m_uptime_ms >= m_timeout_ms
					|| m_state == STATE_CONNECTED)
					kill(SIGTERM);
				std::cout.flush();
				m_log.flush();
				usleep(300);
			}
			if (m_uptime_ms >= m_timeout_ms)
				m_state = STATE_TIMEDOUT;
		}
		while (read(m_pipefd[0], buffer, sizeof(buffer)))
			m_captured_output += string_t(buffer);
	}

	bool				kill(int sig_num = SIGTERM) {
		bool			ret(false);
		if (m_pid != -1)
		{
			m_log << "killing server (pid:" << m_pid << ")..." << std::endl;
			m_log.flush();
			::kill(m_pid, sig_num);
			ret = true;
			m_pid = -1;
		}
		usleep(500);
		return (ret);
	}


	size_t				uptime() const {
		using Ms = std::chrono::milliseconds;
		const Clock::duration since_epoch = Clock::now() - m_starttime_ms;
		return (std::chrono::duration_cast<Ms>(since_epoch).count());
	}

	const Host			&address() const throw() {
		return (m_addr);
	}

	void				state(int s) throw() {
		m_state = s;
	}
	void				add_response_line(const string_t &l) throw () {
		m_response += l;
	}
	string_t			response() const throw() {
		return (m_response);
	}
	int					state() const throw() {
		return (m_state);
	}

	int					status() const throw() {
		return (m_status);
	}

	unsigned			ping_count() const throw() {
		return (m_pings);
	}

	void				ping_count(unsigned c) throw() {
		m_pings = c;
	}

	string_t			path() const throw() {
		return (m_path);
	}
	stream_t			&log() throw() {
		return (m_log);
	}

	void				print_report(OStream &os) {
		switch (m_state)
		{
		case STATE_STARTING:
			os << "server starting";
			break;
		case STATE_CONNECTED:
			os << "connected to " << address();
			break;
		case STATE_TIMEDOUT:
			os << "connection to " << address() << " timed-out!";
			break;
		case STATE_UNKNOWN:
		default:
			os << "process exitted normally";
			break;
		};
		os << "(" << status() << ")" << std::endl;
		std::flush(os);
		os << captured_output() << std::endl;
	}

	static void			ping(launcher<string_t, stream_t> *inst,
							 SocketStream &sock) throw (std::runtime_error) {
		string_t		line;
		string_t		content;
		inst->log() << "[ping#" << inst->ping_count() << "] " << inst->address() << ": ";
		if (sock.open(inst->address().name(), inst->address().port()))
		{
			inst->state(STATE_CONNECTED);
			usleep(300);
			sock << "EHLLO" << std::endl;
			while (sock)
			{
				std::getline(sock, line);
				inst->add_response_line(line);
				usleep(300);
			}
			inst->log() << "succeeded";
			inst->log() << std::endl;
			content = inst->response();
			if (content.empty())
				inst->log() << "empty reply from server!" << std::endl;
			else
				inst->log() << "server replied with ("
							<< inst->response().size() << " bytes"
							<< "): " << inst->response() << std::endl;
		}
		else
		{
			inst->log() << "failed";
		}
		inst->ping_count(inst->ping_count() + 1);
		usleep(300);
	}

	static launcher<string_t, stream_t>	*instance() {
		return (g_inst);
	}

	string_t							captured_output() const throw() {
		return (m_captured_output);
	}
};

template<typename string_t, typename stream_t>
launcher<string_t, stream_t>		*launcher<string_t, stream_t>::g_inst = NULL;

#endif
