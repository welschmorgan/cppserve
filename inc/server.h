/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/04 13:30:07 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/12 18:21:58 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERVER_H
# define	SERVER_H

# include <list>

# include "launch_options.h"
# include "client.h"
# include "address.h"
# include "access_control.h"
# include "locator.h"
# include "stringlist.h"
# include "request.h"
# include "response.h"

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
	Locator								mLocator;
	SharedAccessControlList				mAccessList;
	SharedHTTPResponse					mResponse;

private:
	HTTPServer(const HTTPServer &rk);
	HTTPServer &operator=(const HTTPServer &rk);

public:
	HTTPServer(int argc, char *const argv[]);
	virtual ~HTTPServer();

	int									run();

	void								discoverLocations();

	const Locator						&getLocator() const;
	Locator								&getLocator();


	bool								checkAccessList(SharedHTTPClientPtr client);
	void								closeClients();

	uint16_t							getPort() const throw();
	void								setPort(uint16_t p) throw();

	bool								isShuttingDown() const throw();
	int									shutdown();

	void								serve(SharedHTTPClientPtr client);
	void								handleGetRequest(SharedHTTPClientPtr client,
														 const std::string &path);

	void								dumpSocketStates(std::ostream &os) const;

	SharedAccessControlList				getAccessList() const;

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
