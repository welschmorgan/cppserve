/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/04 13:30:07 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/23 16:54:01 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERVER_H
# define	SERVER_H

# include <list>

# include "launch_options.hpp"
# include "client.hpp"
# include "address.hpp"
# include "access_control.hpp"
# include "locator.hpp"
# include "stringlist.hpp"
# include "request.hpp"
# include "response.hpp"
# include "logger.hpp"
# include "manual.hpp"

typedef std::list<SharedHTTPClientPtr>			SharedHTTPClientList;

class											HTTPServer;
typedef std::list<HTTPServer*>					HTTPServerList;

class									HTTPServer
{
public:
	typedef SharedHTTPClientList		ClientList;
	typedef SharedHTTPClientPtr			ClientPtr;

protected:
	LaunchOptions						mArgs;
	SocketStream						mSocket;
	uint16_t							mPort;
	ClientList							mClients;
	bool								mShutdown;
	bool								mStarted;
	Locator								mLocator;
	SharedAccessControlList				mAccessList;
	SharedHTTPResponse					mResponse;
	URITranslationMap					mTranslations;
	URITranslationResultMap				mResults;
	ETagMap								mTags;
	shared_logger_ptr					mLogger;
	Manual								mManual;

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
	void								buildManual() throw(std::runtime_error);

	const URITranslationMap				*getURITranslationMap() const;
	std::string							translateURI(const std::string &uri);

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

	void								setVerbose(log_level state);
	static void							_onSignal(int no);

	logger								&log(const String t, log_level l = LL_NORMAL)
		{ mLogger->insert(t, l); return (*mLogger); }

	logger								&getLogger() throw();

	void								showHelpScreen(const String &name = String("general")) throw(std::runtime_error);
protected:
	static HTTPServerList				Instances;

};

#endif
