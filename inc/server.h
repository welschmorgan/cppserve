/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/04 13:30:07 by mwelsch           #+#    #+#             */
/*   Updated: 2017/02/05 19:41:04 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERVER_H
# define	SERVER_H

# include <list>

# include "launch_options.h"
# include "client.h"


typedef std::list<SharedHTTPClientPtr>	SharedHTTPClientList;

class									HTTPServer;
typedef std::list<HTTPServer*>			HTTPServerList;

class HTTPServer
{
public:
	typedef SharedHTTPClientList		ClientList;
	typedef SharedHTTPClientPtr			ClientPtr;

protected:
	LaunchOptions						mArgs;
	SocketStream						mSocket;
	uint16_t							mPort;
	ClientList							mClients;
	long								mVerbose;
	bool								mShutdown;

public:
	HTTPServer(int argc, char *const argv[]);
//	HTTPServer(const HTTPServer &rk);
	virtual ~HTTPServer();

	int									run();

	uint16_t							getPort() const throw();
	void								setPort(uint16_t p) throw();

	bool								isShuttingDown() const throw();
	int									shutdown();

	void								serve(SharedHTTPClientPtr client);

	void								onSignal(int no);
	void								onAccept(SocketStream::ptr strm, sockaddr_in *addr);
	void								onReject(SocketStream::ptr strm, sockaddr_in *addr);
	bool								onGuard(SocketStream::ptr strm, sockaddr_in *addr);
	void								onError(sockaddr_in *cli);

	ClientList							&getClients();
	const ClientList					&getClients() const;

	void								setVerbose(bool state);

	static void							_onSignal(int no);
protected:
	static HTTPServerList				Instances;
};

#endif
