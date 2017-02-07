/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/04 13:30:07 by mwelsch           #+#    #+#             */
/*   Updated: 2017/02/07 21:44:25 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERVER_H
# define	SERVER_H

# include <list>

# include "launch_options.h"
# include "client.h"

typedef std::vector<std::string>		StringList;

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
	std::string							mBaseDir;
	ClientList							mClients;
	long								mVerbose;
	bool								mShutdown;

private:
	HTTPServer(const HTTPServer &rk);
	HTTPServer &operator=(const HTTPServer &rk);

public:
	HTTPServer(int argc, char *const argv[]);
	virtual ~HTTPServer();

	int									run();

	void								closeClients();
	std::string							getBaseDir() const;
	void								setBaseDir(const std::string &d);


	uint16_t							getPort() const throw();
	void								setPort(uint16_t p) throw();

	bool								isShuttingDown() const throw();
	int									shutdown();

	void								serve(SharedHTTPClientPtr client);

	StringList							getMethods() const;

	bool								isMethodName(const std::string &name);
	void								parseRequest(SharedHTTPClientPtr client,
													 const std::string &method,
													 const std::string &uri,
													 const std::string &proto);

	void								dumpSocketStates(std::ostream &os) const;

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
