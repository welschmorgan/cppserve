/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/05 13:28:49 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/05 17:56:15 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

/*-----------------------------------------------------------------.
  | Copyright (C) 2011 Galik grafterman@googlemail.com               |
  '------------------------------------------------------------------'

  This code is was created from code (C) Copyright Nicolai M. Josuttis 2001
  with the following Copyright Notice:
*/

/* The following code declares classes to read from and write to
 * file descriptore or file handles.
 *
 * See
 *      http://www.josuttis.com/cppcode
 * for details and the latest version.
 *
 * - open:
 *      - integrating BUFSIZ on some systems?
 *      - optimized reading of multiple characters
 *      - stream for reading AND writing
 *      - i18n
 *
 * (C) Copyright Nicolai M. Josuttis 2001.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 * Version: Jul 28, 2002
 * History:
 *  Jul 28, 2002: bugfix memcpy() => memmove()
 *                fdinbuf::underflow(): cast for return statements
 *  Aug 05, 2001: first public version
 */

/*

  Permission to copy, use, modify, sell and distribute this software
  is granted under the same conditions.

  '----------------------------------------------------------------*/
#ifndef		SOCKET_H
# define	SOCKET_H

# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>
# include <unistd.h>
# include <string.h>
# include <strings.h>
# include <streambuf>
# include <istream>
# include <ostream>
# include <memory>
# include <iostream>

# define MAX_HTTP_CLIENTS				100
# define MAX_HTTP_PENDING_CONNECTIONS	5

template<typename Char>
class						BasicSocketBuffer
: public std::basic_streambuf<Char>
{
public:
	typedef Char							char_type;
	typedef std::basic_streambuf<char_type>	buf_type;
	typedef std::basic_ostream<char_type>	stream_type;
	typedef typename buf_type::int_type		int_type;
	typedef typename std::basic_streambuf<Char>::traits_type traits_type;

protected:

	static const int		char_size = sizeof(char_type);
	static const int		SIZE = 128;
	char_type				obuf[SIZE] ;
	char_type				ibuf[SIZE] ;

	int						sock;

public:
	BasicSocketBuffer()
		: sock(0)
	{
		buf_type::setp(obuf, obuf + (SIZE - 1));
		buf_type::setg(ibuf, ibuf, ibuf);
	}
	virtual					~BasicSocketBuffer() {
		sync();
	}

	void					set_socket(int sock) { this->sock = sock; }
	int						get_socket() { return this->sock; }

protected:

	int						output_buffer() {
		int num = buf_type::pptr() - buf_type::pbase();
		if(send(sock, reinterpret_cast<char*>(obuf), num * char_size, 0) != num)
			return traits_type::eof();
		buf_type::pbump(-num);
		return num;
	}

	virtual int_type		overflow(int_type c)
	{
		if(c != traits_type::eof())
		{
			*buf_type::pptr() = c;
			buf_type::pbump(1);
		}

		if(output_buffer() == traits_type::eof())
			return traits_type::eof();
		return c;
	}

	virtual int				sync()
	{
		if(output_buffer() == traits_type::eof())
			return traits_type::eof();
		return 0;
	}

	virtual int_type		underflow()
	{
		if(buf_type::gptr() < buf_type::egptr())
			return *buf_type::gptr();

		int num;
		if((num = recv(sock, reinterpret_cast<char*>(ibuf), SIZE * char_size, 0)) <= 0)
			return traits_type::eof();

		buf_type::setg(ibuf, ibuf, ibuf + num);
		return *buf_type::gptr();
	}
};

typedef BasicSocketBuffer<char>		SocketBuf;
typedef BasicSocketBuffer<wchar_t>	WSocketBuf;

template<typename Char>
class								BasicSocketStream
: public std::basic_iostream<Char>
{
public:
	typedef Char char_type;
	typedef std::basic_iostream<char_type> stream_type;
	typedef BasicSocketBuffer<char_type> buf_type;
	typedef std::shared_ptr<BasicSocketStream> ptr;

	typedef std::function<void(ptr strm, sockaddr_in *addr)>	ConnectionHandler;
	typedef std::function<bool(ptr strm, sockaddr_in *addr)>	ConnectionGuard;
	typedef std::function<void(sockaddr_in *cli)>			ErrorHandler;

protected:
	buf_type						buf;
	std::string						msg;

public:
	BasicSocketStream() : stream_type(&buf), msg() {}
	BasicSocketStream(int s) : stream_type(&buf), msg() { buf.set_socket(s); }

	bool							open(const std::string& host, uint16_t port) {
		close();
		int sd = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in sin;
		hostent *he = gethostbyname(host.c_str());
		int code = errno;

		std::copy(reinterpret_cast<char*>(he->h_addr)
				  , reinterpret_cast<char*>(he->h_addr) + he->h_length
				  , reinterpret_cast<char*>(&sin.sin_addr.s_addr));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);

		if(connect(sd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) < 0) {
			stream_type::setstate(std::ios::failbit);
			code = errno;
		} else {
			buf.set_socket(sd);
		}
		msg = strerror(errno);
		return *this;
	}

	std::string						message() const {
		return (msg);
	}

	bool							setOption(int level, int name,
											  void *val, socklen_t len) {
		return (setsockopt(buf.get_socket(), level, name, val, len) < 0);
	}

	bool							getOption(int level, int name,
											  const void *val, socklen_t len) {
		return (getsockopt(buf.get_socket(), level, name, val, len));
	}

	void							setOptions() throw(std::exception) {
		int							enable = 1;
		if (!setOption(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
			throw std::runtime_error("failed to set option: SO_REUSEADDR: " + std::string(strerror(errno)));
		if (!setOption(SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)))
			throw std::runtime_error("failed to set option: SO_REUSEPORT: " + std::string(strerror(errno)));
	}

	bool							listen(uint16_t port) {
		close();
		int code = 0;
		bool ret = true;
		int sd = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in sin;
		msg = "";
		if (sd < 0) {
			msg = "failed to create socket";
			code = errno;
			ret = false;
		} else {
			bzero((char *) &sin, sizeof(sin));
			sin.sin_addr.s_addr = INADDR_ANY;
			sin.sin_family = AF_INET;
			sin.sin_port = htons(port);
			setOptions();
			if(bind(sd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
				ret = false;
				code = errno;
				msg = "failed to bind socket";
			} else {
				buf.set_socket(sd);
				ret = !(::listen(sd, MAX_HTTP_PENDING_CONNECTIONS) < 0);//
				if (!ret) {
					msg = "failed to listen";
					code = errno;
				}
			}
		}
		if (!ret) {
			char *e = strerror(code);
			msg += ":" + std::string(e, e + strlen(e));
			stream_type::setstate(std::ios::failbit);
		}
		return (ret);
	}

	std::shared_ptr<BasicSocketStream>		accept(ConnectionHandler acceptor,
												   ConnectionHandler rejector,
												   ConnectionGuard guard,
												   ErrorHandler errorHandler) {
		std::shared_ptr<BasicSocketStream> strm;
		int sockFd;
		int clientFd;
		sockaddr_in cliAddr;
		unsigned int clientLen(sizeof(cliAddr));

		sockFd = buf.get_socket();
		if (sockFd >= 0) {
			clientFd = ::accept(sockFd,
								reinterpret_cast<sockaddr*>(&cliAddr),
								&clientLen);
			if (clientFd < 0) {
				errorHandler(&cliAddr);
				return (strm);
			}
			strm.reset(new BasicSocketStream(clientFd));
			if (guard(strm, &cliAddr)) {
				acceptor(strm, &cliAddr);
			} else {
				rejector(strm, &cliAddr);
			}
		}
		return (strm);
	}

	void							close() {
		if (buf.get_socket() != 0) {
			if (!::close(buf.get_socket())) {
				buf.set_socket(0);
			}
		}
		stream_type::clear();
	}
};

typedef BasicSocketStream<char>		SocketStream;
typedef BasicSocketStream<wchar_t>	WSocketStream;

#endif
