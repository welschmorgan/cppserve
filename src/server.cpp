// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   server.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/04 13:43:08 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/29 19:54:15 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "server.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <arpa/inet.h>
#include "logger.hpp"

HTTPServerList					HTTPServer::Instances = HTTPServerList();


static std::string				GetTime()
{
	time_t			rawtime;
	struct tm *		timeinfo;
	char			buffer[80];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (buffer, 80, "%c %Z", timeinfo);
	return (buffer);
}

static std::string	&TrimQuotes(std::string &str)
{
	while (!str.empty() && (str.at(0) == ' ' || str.at(0) == '\r' || str.at(0) == '\n' || str.at(0) == '\t'))
		str.erase(str.begin());
	while (!str.empty() && (str.at(str.size() - 1) == ' ' || str.at(str.size() - 1) == '\t' || str.at(str.size() - 1) == '\r' || str.at(str.size() - 1) == '\n'))
		str.erase(str.begin() + (str.size() - 1));
	if (!str.empty() && str.at(0) == '"')
		str.erase(str.begin());
	if (!str.empty() && str.at(str.size() - 1) == '"')
		str.erase(str.begin() + (str.size() - 1));
	return (str);
}

static void						option_quiet(void *data) {
	HTTPServer *serv = reinterpret_cast<HTTPServer *>(data);
	serv->setVerbose(LL_LOWEST);
}
static void						option_help_usage(void *data) {
	HTTPServer *serv = reinterpret_cast<HTTPServer *>(data);
	serv->showHelpScreen("usage");
}
static void						option_verbose(void *data) {
	HTTPServer *serv = reinterpret_cast<HTTPServer *>(data);
	serv->setVerbose(LL_NORMAL);
}
static void						option_verbose_high(void *data) {
	HTTPServer *serv = reinterpret_cast<HTTPServer *>(data);
	serv->setVerbose(LL_HIGH);
}
static void						option_verbose_highest(void *data) {
	HTTPServer *serv = reinterpret_cast<HTTPServer *>(data);
	serv->setVerbose(LL_HIGHEST);
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
	, mShutdown(false)
	, mStarted(false)
	, mLocator()
	, mAccessList(new AccessControlList())
	, mResponse(new HTTPResponse())
	, mTranslations()
	, mResults()
	, mTags()
	, mLogger()
	, mManual()
{
	Instances.push_back(this);
	HTTPStatus::init();
	mLogger.reset(new file_logger("server.log",
								  LL_HIGHEST,
								  LF_LEVEL | LF_TRUNCATE | LF_TIMESTAMP));

	mArgs.set("help:general",
			  StringList{"-h", "--help"},
			  std::bind(option_help_usage, std::placeholders::_1),
			  "",
			  "shows this help screen");
	mArgs.set("verbose:quiet",
			  StringList{"-q", "--quiet"},
			  std::bind(option_quiet, std::placeholders::_1),
			  "",
			  "shows additionnal log messages");
	mArgs.set("verbose:normal",
			  StringList{"-v", "--verbose"},
			  std::bind(option_verbose, std::placeholders::_1),
			  "",
			  "shows additionnal log messages");
	mArgs.set("verbose:high",
			  StringList{"-vv", "--high-verbose"},
			  std::bind(option_verbose_high, std::placeholders::_1),
			  "",
			  "shows additionnal log messages");
	mArgs.set("verbose:talkative",
			  StringList{"-vvv", "--talkative"},
			  std::bind(option_verbose_highest, std::placeholders::_1),
			  "",
			  "shows additionnal log messages");
	buildManual();
	mArgs.parse((void*)this);
	ManualSection *sec = mManual.getActiveSection();
	if (!sec)
	{
		static char buf[1024] = {0};
		getcwd(&buf[0], 1024);

		mTranslations["/"] = URITranslation{ "index.html", "index.php", "index.js" };

		mLocator.setBaseDir(buf);
		mLocator.addHandler(SharedResourceScanHandler(new StaticResourceHandler(&mLocator)));
		mLocator.addHandler(SharedResourceScanHandler(new URITranslationHandler(&mLocator,
																				&mTranslations, mResults)));
		mLocator.addHandler(SharedResourceScanHandler(new ETagHandler(&mLocator, mTags)));
		mLocator.addHandler(SharedResourceScanHandler(new ACLResourceHandler(&mLocator)));
		discoverLocations();
	}
}

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


void								HTTPServer::buildManual() throw(std::runtime_error)
{

	ManualSection							*sec = NULL;
	LaunchOptions::ArgList					*args;
	std::vector<std::string>::const_iterator trig;
	String			usageString("cppserve");

	usageString += " [OPTIONS...]";
	sec = mManual.createSection("general",
								"General Section",
								StringStream(),
								"Morgan Welsch",
								StringStream());
	sec->desc << "General use of the web server";
	sec->lines << "To start the server just type at a command prompt 'cppserve'";
	sec = mManual.createSection("usage",
								"Usage Section",
								StringStream(),
								"Morgan Welsch",
								StringStream());
	sec->desc << "Options and command-line usage";
	sec->lines << usageString << std::endl;
	sec->lines << std::endl;
	sec->lines << "options:" << std::endl;
	sec->lines << String(8, '-') << std::endl;
	args = mArgs.registered();
	String		triggerString;
	for (auto arg : *args)
	{
		triggerString = "";
		for (trig = arg.getTriggers().begin();
			 trig != arg.getTriggers().end();
			 trig++)
		{
			if (trig != arg.getTriggers().begin())
				triggerString += ", ";
			triggerString += *trig;
		}
		triggerString += ":";
		sec->lines << String(1, '\t') << triggerString << std::endl;
		sec->lines << String(1, '\t') << String(triggerString.size(), '-') << std::endl;
		sec->lines << String(2, '\t') << arg.getDesc() << std::endl;

	}
}

logger								&HTTPServer::getLogger() throw()
{
	return (*mLogger);
}

int									HTTPServer::run()
{
	ManualSection *sec = mManual.getActiveSection();
	if (sec)
	{
		mLogger->flags(LF_NONE);
		(*mLogger) << *sec << std::endl;
		return (EXIT_FAILURE);
	}
	mStarted = true;
	signal(SIGINT, &HTTPServer::_onSignal);
	mSocket.listen(mPort);
	if (!mSocket) {
		throw std::runtime_error(mSocket.message());
	}
	(*mLogger) << "[+] Starting server ..." << std::endl;
	(*mLogger) << "\thttp://localhost:" << mPort << std::endl;
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

void								HTTPServer::closeClients() {
	SharedHTTPClientList::iterator	cit;
	for (cit = mClients.begin(); cit != mClients.end(); cit++) {
		if (*cit)
			(*cit)->close();
	}
	if (mSocket.isOpen()) {
		(*mLogger) << "[+] Closing master socket" << std::endl;
		mSocket.close();
	}
}

const Locator						&HTTPServer::getLocator() const
{ return (mLocator); }
Locator								&HTTPServer::getLocator()
{ return (mLocator); }


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
	if (mStarted && !mShutdown)
	{
		if (mLogger->level() >= LL_NORMAL) {
			dumpSocketStates(mLogger->stream());
			(*mLogger) << "[+] Shutting down ..." << std::endl;
		}
		closeClients();
	}
	mShutdown = true;
	return (0);
}

void								HTTPServer::dumpSocketStates(std::ostream &os) const {
	SharedHTTPClientList::const_iterator it;
	for (it = mClients.begin(); it != mClients.end(); it++) {
		os << (*it)->getAddress() << "\t" << (*it)->getStream()->getStage() << std::endl;
	}
}

void								HTTPServer::onSignal(int no) {
	(*mLogger) << "[+] Caught: " << no << std::endl;
	if (no == SIGINT || no == SIGTERM || no == SIGQUIT) {
		shutdown();
		//exit(0);
	}
}

void							HTTPServer::_onSignal(int no) {
	HTTPServerList::iterator		it;
	for (it = Instances.begin(); it != Instances.end(); it++) {
		(*it)->onSignal(no);
	}
}

void							HTTPServer::onAccept(SocketStream::ptr strm, sockaddr_in *addr) {
	(*mLogger) << "[+] Connection Accepted: " << *strm << std::endl;
	Address						raddr(addr);
	mClients.push_back(SharedHTTPClientPtr(new HTTPClient(strm, raddr)));
	SharedHTTPClientPtr client(mClients.back());
	//pid_t pid = fork();
	//if (pid == 0) {
	//client->_setPID((int)pid);
		serve(client);
		(*mLogger) << "[+] Done: " << *client << std::endl;
		//client->_setPID(0);
		//exit(0);
		//} else {
		client->close();
		//}
}

bool							HTTPServer::checkAccessList(SharedHTTPClientPtr client)
{
	AccessControlList::SectionList::const_iterator it;
	StringList::const_iterator lt;
	StringMap::const_iterator mt2;
	StringMap::const_iterator mt;
	bool inc = false, exc = false;
	(*mLogger) << "[+] Checking acl ..." << std::endl;
	for (it = mAccessList->getSections()->begin(); !inc && !exc && it != mAccessList->getSections()->end(); it++) {
		if (it->second->getMode() == ACM_INCLUDE) {
			if (it->second->getName() == client->getRequest()->getURI()) {
				(*mLogger) << "* check acl section: '" << client->getRequest()->getURI() << "' == '" << it->second->getName() << "'" << std::endl;
				for (lt = it->second->getURIs().begin(); !inc && !exc && lt != it->second->getURIs().end(); lt++)
				{
					(*mLogger) << "\turi matches: " << client->getRequest()->getURI() << std::endl;
					if (*lt == client->getRequest()->getURI()) {
						inc = true;
						break ;
					}
				}
				for (mt = it->second->getHeaders().begin(); !inc && !exc && mt != it->second->getHeaders().end(); mt++)
				{
					for (mt2 = client->getRequest()->getHeaders()->begin(); !inc && !exc && mt2 != client->getRequest()->getHeaders()->end(); mt2++) {
						(*mLogger) << "\theader matches: " << mt->first << ": " << mt2->second << " == " << mt->second << std::endl;
						if (mt2->first == mt->first) {
							inc = true;
							break ;
						}
					}
				}
			}
		} else if (it->second->getMode() == ACM_EXCLUDE) {
			if (it->second->getName() == client->getRequest()->getURI()) {
				(*mLogger) << "* check acl section: '" << client->getRequest()->getURI() << "' == '" << it->second->getName() << "'" << std::endl;
				for (lt = it->second->getURIs().begin(); lt != it->second->getURIs().end(); lt++)
				{
					(*mLogger) << "\turi matches: " << client->getRequest()->getURI() << std::endl;
					if (*lt == client->getRequest()->getURI()) {
						exc = true;
						break ;
					}
				}
				for (mt = it->second->getHeaders().begin(); !inc && !exc && mt != it->second->getHeaders().end(); mt++)
				{
					for (mt2 = client->getRequest()->getHeaders()->begin(); !inc && !exc && mt2 != client->getRequest()->getHeaders()->end(); mt2++) {
						(*mLogger) << "\theader matches: " << mt->first << ": " << mt2->second << " == " << mt->second << std::endl;
						if (mt2->first == mt->first) {
							exc = true;
							break ;
						}
					}
				}
			}
		}
	}
	bool ret = ((!inc && !exc) || (!exc || inc));
	(*mLogger) << (ret ? "[+] access granted for client " : "[-] access denied for client ") << *client << std::endl;
	return (ret);
}
SharedAccessControlList			HTTPServer::getAccessList() const {
	return (mAccessList);
}

void							HTTPServer::discoverLocations() {
	mLocator.discover(this);
}

void							HTTPServer::handleGetRequest(SharedHTTPClientPtr client,
															 const std::string &path) {
	std::ifstream		ifs;
	std::stringstream	content;
	std::string			line(255, 0);
	std::string			etagValue("");
	bool				foundEtagVerif(false);
	SharedHTTPRequest	req(client->getRequest());
	HTTPProtocol		proto(req->getProtocol());
	std::string			lastEtagValue(req->getHeader("If-None-Match",
													 &foundEtagVerif));
	(*mLogger) << "[+] serving file: " << path << std::endl;
	TrimQuotes(lastEtagValue);
	if (!checkAccessList(client))
	{
		mResponse->setStatus(HTTPStatus::Client::Unauthorized);
		content << HTTPStatus::CodeName(HTTPStatus::Client::Unauthorized) << std::endl;
	}
	else
	{
		etagValue = mTags[path];
		if (foundEtagVerif && etagValue == lastEtagValue)
			mResponse->setStatus(HTTPStatus::Redirect::NotModified);
		else
		{
			ifs.open(path);
			if (!ifs)
			{
				mResponse->setStatus(HTTPStatus::Client::NotFound);
				content << "The requested URI wasn't found!" << std::endl;
			}
			else
			{
				while (ifs >> line)
					content << line << std::endl;
				ifs.close();
				std::hash<std::string> hasher;
				etagValue = std::to_string(hasher(content.str()));
				mResponse->setStatus(HTTPStatus::Success::Ok);
			}
		}
	}
	mResponse->setBody(content.str());
	mTags[path] = etagValue;
	mResponse->setHeader("ETag", "\"" + mTags[path] + "\"");
}


void							HTTPServer::serve(SharedHTTPClientPtr client) {
	std::string					filePath;
	std::string					lastModif = "Wed, 18 Jun 2003 16:05:58 GMT";
	std::string					ctype = "text/html";
	std::string					servName = "Apache/2.2.3";


	try {
		(*mLogger) << "[+] Serving: " << *client << std::endl;
		client->parseRequest();
		mResponse->reset(client->getRequest()->getProtocol());
		mResponse->setHeader("Date", GetTime());
		mResponse->setHeader("Server" , servName);
		mResponse->setHeader("Last-Modified" , lastModif);
		mResponse->setHeader("Content-Type", ctype);
		mResponse->setHeader("Accept-Ranges", "bytes");
		mResponse->setHeader("Connection", "Close");

		if (client->getRequest()->getMethod() == "GET")
		{
			filePath = translateURI(client->getRequest()->getProtocol().getURI());
			if (filePath.empty())
				filePath = client->getRequest()->getProtocol().getURI();
			handleGetRequest(client, filePath);
		}
	} catch (std::exception &ex) {
		mResponse->setStatus(HTTPStatus::Server::InternalError);
		mResponse->setBody(ex.what());
		(*mLogger) << ex.what() << std::endl;
	}
	mResponse->setHeader("Content-Length", std::to_string(mResponse->getBody().size()));
	client->writeResponse(mResponse);
	client->close();
}

const URITranslationMap			*HTTPServer::getURITranslationMap() const {
	return (&mTranslations);
}

std::string						HTTPServer::translateURI(const std::string &uri)
{
	URITranslationResultMap::const_iterator	it;
	std::string								ret;
	if ((it = mResults.find(uri)) != mResults.end())
		ret = it->second;
	return (ret);
}

void							HTTPServer::onReject(SocketStream::ptr strm, sockaddr_in *addr) {
	(*mLogger) << "[+] Connection Rejected: " << *strm << std::endl;
	(void)addr;
}

bool							HTTPServer::onGuard(SocketStream::ptr strm, sockaddr_in *addr) {
	bool ret = true;
	(*mLogger) << "[+] Connection guard: " << *strm << std::endl;
	if (mClients.size() >= MAX_HTTP_CLIENTS) {
		ret = false;
	}
	(void)addr;
	return (ret);
}

void							HTTPServer::onError(sockaddr_in *cli) {
	if (!mShutdown) {
		(*mLogger) << "[-] Connection Error: " << cli << std::endl;
		if (!mSocket.message().empty()) {
			(*mLogger) << mSocket.message() << std::endl;
		}
		perror("");
	}
}

bool							HTTPServer::isShuttingDown() const throw() {
	return (mShutdown);
}

void							HTTPServer::setVerbose(log_level state)
{ mLogger->level(state); }


void							HTTPServer::showHelpScreen(const String &name) throw(std::runtime_error)
{
	mManual.activateSection(name);
}
