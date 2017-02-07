// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   server.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/04 13:43:08 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/05 20:07:54 by mwelsch          ###   ########.fr       //
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
	, mClients()
	, mVerbose(true)
	, mShutdown(false)
{
	Instances.push_back(this);
	const std::vector<std::string> verbose{"v", "verbose"};
	mArgs.set("verbose", verbose, std::bind(option_verbose, this),
			  "");
	mArgs.parse();
}

/*HTTPServer::HTTPServer(const HTTPServer &rk)
  { (void) rk; }*/

HTTPServer::~HTTPServer()
{
	SharedHTTPClientList::iterator	cit;
	for (cit = mClients.begin(); cit != mClients.end(); cit++) {
		(*cit)->close();
	}
	mSocket.close();
	HTTPServerList::iterator		it;
	for (it = Instances.begin(); it != Instances.end(); it++) {
		if (*it == this) {
			Instances.erase(it);
			break ;
		}
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
	std::cout << "[+] Shutting down ..." << std::endl;
	mShutdown = true;
	SharedHTTPClientList::iterator	cit;
	for (cit = mClients.begin(); cit != mClients.end(); cit++) {
		(*cit)->close();
	}
	mSocket.close();
	return (0);
}

int									HTTPServer::run()
{
	signal(SIGINT, &HTTPServer::_onSignal);
	mSocket.listen(mPort);
	if (!mSocket) {
		throw std::runtime_error(mSocket.message());
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
	pid_t pid = fork();
	if (pid == 0) {
		client->_setPID((int)pid);
		serve(client);
		std::cout << "[+] Done: " << *client << std::endl;
		client->_setPID(0);
		exit(0);
	} else {
		client->close();
	}
}

void							HTTPServer::serve(SharedHTTPClientPtr client) {
	std::cout << "[+] Spawned: " << *client << std::endl;
	SocketStream &stream(*client->getStream());
	stream << "HELLO!" << std::endl;
	std::string data;
	static char buf[1024] = {0};
	std::string cwd(getcwd(&buf[0], 1024));
	while (stream >> data) {
		if (data == "GET") {
			stream >> data;
			std::cout << "* serving file: " << cwd << data << std::endl;
		}
		std::cout << data << std::endl;
	}
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
