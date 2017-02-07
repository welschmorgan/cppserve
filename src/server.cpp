// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   server.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/04 13:43:08 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/07 21:57:12 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "server.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <signal.h>
#include <arpa/inet.h>

HTTPServerList					HTTPServer::Instances = HTTPServerList();

static void						option_verbose(HTTPServer *serv) {
	serv->setVerbose(true);
}

static void						server_accept(HTTPServer *serv, SocketStream::ptr strm, sockaddr_in *addr) {
	serv->onAccept(strm, addr);
}
static void						server_reject(HTTPServer *serv, SocketStream::ptr strm, sockaddr_in *addr) {
	serv->onReject(strm, addr);
}
static bool						server_guard(HTTPServer *serv, SocketStream::ptr strm, sockaddr_in *addr) {
	return (serv->onGuard(strm, addr));
}
static void						server_error(HTTPServer *serv, sockaddr_in *cli) {
	serv->onError(cli);
}

HTTPServer::HTTPServer(int argc, char *const argv[])
	: mArgs(argc, argv)
	, mSocket(0, 0)
	, mPort(8080)
	, mBaseDir()
	, mClients()
	, mVerbose(true)
	, mShutdown(false)
{
	Instances.push_back(this);
	const StringList verbose{"v", "verbose"};
	mArgs.set("verbose", verbose, std::bind(option_verbose, this),
			  "");
	mArgs.parse();
	static char buf[1024] = {0};
	mBaseDir = std::string(getcwd(&buf[0], 1024));
}

/*HTTPServer::HTTPServer(const HTTPServer &rk)
  { (void) rk; }*/

HTTPServer::~HTTPServer()
{
	shutdown();
	HTTPServerList::iterator		it;
	for (it = Instances.begin(); it != Instances.end(); it++) {
		if (*it == this) {
			Instances.erase(it);
			break ;
		}
	}
}

void								HTTPServer::closeClients() {
	SharedHTTPClientList::iterator	cit;
	for (cit = mClients.begin(); cit != mClients.end(); cit++) {
		if (*cit)
			(*cit)->close();
	}
	if (mSocket.isOpen()) {
		std::cout << "[+] Closing master socket" << std::endl;
		mSocket.close();
	}
}

HTTPServer::ClientList				&HTTPServer::getClients() {
	return (mClients);
}

const HTTPServer::ClientList		&HTTPServer::getClients() const {
	return (mClients);
}

uint16_t							HTTPServer::getPort() const throw() {
	return (mPort);
}
void								HTTPServer::setPort(uint16_t p) throw() {
	mPort = p;
}

int									HTTPServer::shutdown() {
	if (mVerbose) {
		dumpSocketStates(std::cout);
		std::cout << "[+] Shutting down ..." << std::endl;
	}
	closeClients();
	mShutdown = true;
	return (0);
}

void								HTTPServer::dumpSocketStates(std::ostream &os) const {
	SharedHTTPClientList::const_iterator it;
	//os << "address" << "\t" << "stage" << std::endl;
	for (it = mClients.begin(); it != mClients.end(); it++) {
		os << (*it)->getAddress(true) << "\t" << (*it)->getStream()->getStage() << std::endl;
	}
}

int									HTTPServer::run()
{
	signal(SIGINT, &HTTPServer::_onSignal);
	mSocket.listen(mPort);
	if (!mSocket) {
		throw std::runtime_error(mSocket.message());
	}
	if (mVerbose) {
		std::cout << "[+] Starting server ..." << std::endl;
		std::cout << "\thttp://localhost:" << mPort << std::endl;
	}
	while (!mShutdown) {
		mSocket.accept(std::bind(server_accept,
								 this, std::placeholders::_1, std::placeholders::_2),
					   std::bind(server_reject,
								 this, std::placeholders::_1, std::placeholders::_2),
					   std::bind(server_guard,
								 this, std::placeholders::_1, std::placeholders::_2),
					   std::bind(server_error,
								 this, std::placeholders::_1));
	}
	return (EXIT_SUCCESS);
}

void							HTTPServer::onSignal(int no) {
	if (mVerbose) {
		std::cerr << "[+] Caught: " << no << std::endl;
	}
	if (no == SIGINT) {
		shutdown();
		exit(0);
	}
}

void							HTTPServer::_onSignal(int no) {
	HTTPServerList::iterator		it;
	for (it = Instances.begin(); it != Instances.end(); it++) {
		(*it)->onSignal(no);
	}
}

void							HTTPServer::onAccept(SocketStream::ptr strm, sockaddr_in *addr) {
	std::cout << "[+] Connection Accepted: " << *strm << std::endl;
	short						family = AF_INET;
	std::string					saddr("0.0.0.0");
	uint16_t					port(0);
	static char					buf[32] = { 0 };
	if (addr) {
		family = addr->sin_family;
		inet_ntop(family, (void*)addr, &buf[0], 32);
		saddr = std::string(&buf[0], &buf[0] + 32);
		port = addr->sin_port;
	}
	mClients.push_back(SharedHTTPClientPtr(new HTTPClient(strm,
														  family,
														  saddr,
														  port)));
	SharedHTTPClientPtr client(mClients.back());
	//pid_t pid = fork();
	//if (pid == 0) {
	//client->_setPID((int)pid);
		serve(client);
		std::cout << "[+] Done: " << *client << std::endl;
		//client->_setPID(0);
		//exit(0);
		//} else {
		client->close();
		//}
}


StringList		HTTPServer::getMethods() const {
	static StringList			methods;
	static const char			cmethods[][10] = {
		"PUT", "POST", "PATCH",
		"DELETE", "UPDATE",
		"OPTION",
		"HEAD",
		"GET"
	};
	if (methods.empty()) {
		methods = StringList(cmethods, cmethods + 8);
	}
	return (methods);
}

bool							HTTPServer::isMethodName(const std::string &name) {
	const StringList			methods(getMethods());
	StringList::const_iterator	it;
	bool						ret = false;
	for (it = methods.begin(); !ret && it != methods.end(); it++) {
		if ((*it) == name) {
			ret = true;
		}
	}
	return (ret);
}

void							HTTPServer::parseRequest(SharedHTTPClientPtr client,
														 const std::string &method,
														 const std::string &uri,
														 const std::string &proto) {
	SocketStream				&strm(*client->getStream());
	std::cout << "* serving file: " << mBaseDir << "/" << uri << std::endl;
}


std::string						HTTPServer::getBaseDir() const {
	return (mBaseDir);
}

void							HTTPServer::setBaseDir(const std::string &d) {
	mBaseDir = d;
}

void							HTTPServer::serve(SharedHTTPClientPtr client) {
	if (mVerbose) {
		std::cout << "[+] Spawned: " << *client << std::endl;
	}
	SocketStream &stream(*client->getStream());
	std::map<std::string, std::string> lines;
	static char buf[1024] = {0};
	if (!(stream >> lines["method"]) || !isMethodName(lines["method"])) {
		return ;
	}
	if (!(stream >> lines["uri"])) {
		return ;
	}
	if (!(stream >> lines["proto"])) {
		return ;
	}
	parseRequest(client, lines["method"], lines["uri"], lines["proto"]);
	stream << "HELLO!" << std::endl;
}

void							HTTPServer::onReject(SocketStream::ptr strm, sockaddr_in *addr) {
	std::cout << "[+] Connection Rejected: " << *strm << std::endl;
}

bool							HTTPServer::onGuard(SocketStream::ptr strm, sockaddr_in *addr) {
	bool ret = true;
	std::cout << "[+] Connection guard: " << *strm << std::endl;
	if (mClients.size() >= MAX_HTTP_CLIENTS) {
		ret = false;
	}
	return (ret);
}

void							HTTPServer::onError(sockaddr_in *cli) {
	if (!mShutdown) {
		std::cerr << "[-] Connection Error: " << cli << std::endl;
		if (!mSocket.message().empty()) {
			std::cerr << mSocket.message() << std::endl;
		}
		perror("");
	}
}

bool							HTTPServer::isShuttingDown() const throw() {
	return (mShutdown);
}

void							HTTPServer::setVerbose(bool state)
{ mVerbose = state; }
