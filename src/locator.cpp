// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   locator.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/10 21:41:12 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/23 14:44:39 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "locator.hpp"
#include <iomanip>
#include <sys/types.h>
#include <dirent.h>
#include "access_control.hpp"
#include "server.hpp"

Locator::Locator(const std::string &base_dir,
				 shared_logger_ptr logger)
	: mBaseDir(base_dir)
	, mFolders(new StringList())
	, mFiles(new StringList())
	, mErrors(new StringList())
	, mHandlers()
	, mExtra(NULL)
	, mLogger(logger)
{
	if (!mBaseDir.empty() && mBaseDir[mBaseDir.size() - 1] == '/') {
		mBaseDir.erase(mBaseDir.end() - 1);
	}
}
Locator::Locator(const Locator &rk)
	: mBaseDir(rk.mBaseDir)
	, mFolders(rk.mFolders)
	, mFiles(rk.mFiles)
	, mErrors(rk.mErrors)
	, mHandlers(rk.mHandlers)
	, mExtra(rk.mExtra)
	, mLogger(rk.mLogger)
{}
Locator::~Locator()
{}

Locator				&Locator::operator=(const Locator &rk)
{
	mBaseDir = rk.mBaseDir;
	mFolders = rk.mFolders;
	mFiles = rk.mFiles;
	mErrors = rk.mErrors;
	mHandlers = rk.mHandlers;
	mExtra = rk.mExtra;
	mLogger = rk.mLogger;
	return (*this);
}

SharedResourceScanHandler			Locator::addHandler(SharedResourceScanHandler h) {
	mHandlers.push_back(h);
	return (mHandlers.back());
}

void								Locator::setHandlers(const ResourceScanHandlerList &l) {
	mHandlers = l;
}
ResourceScanHandlerList				&Locator::getHandlers() {
	return (mHandlers);
}
const ResourceScanHandlerList		&Locator::getHandlers() const {
	return (mHandlers);
}
SharedStringList					Locator::getErrors() const {
	return (mErrors);
}

void								Locator::discoverFolder(const std::string &folder) {
	DIR *e;
	struct dirent *ep;
	std::string name;

	e = opendir(folder.c_str());
	if (!e) {
		mErrors->push_back(folder + ": no such file or directory");
		return ;
	}
	while ((ep = readdir(e))) {
		name = ep->d_name;
		if (name != "." && name != "..") {
			if (ep->d_type != DT_DIR) {
				mFiles->push_back(folder + "/" + name);
				onFileAdded(mFiles->end() - 1);
			} else {
				mFolders->push_back(folder + "/" + name);
				onFolderAdded(mFiles->end() - 1);
			}
			if (ep->d_type == DT_DIR)
				discoverFolder(folder + "/" + name);
		}
	}
}

void				Locator::onFolderAdded(StringList::iterator str) {
	(void)str;
}

void				Locator::onFileAdded(StringList::iterator str) {
	ResourceScanHandlerList::iterator it;
	SharedResourceScanHandler h;
	for (it = mHandlers.begin(); it != mHandlers.end(); it++) {
		h = *it;
		if (h && h->shouldHandle(*str)) {
			(*h)(this, mFiles, str, mExtra);
		}
	}
}

void				*Locator::getExtra() {
	return (mExtra);
}
const void			*Locator::getExtra() const {
	return (mExtra);
}
void				Locator::setExtra(void *e) {
	mExtra = e;
}

void				Locator::discover(void *extra) {
	if (extra)
		mExtra = extra;
	discoverFolder(mBaseDir);
}

void				Locator::setBaseDir(const std::string &dir)
{ mBaseDir = dir; }

const std::string	&Locator::getBaseDir() const
{ return (mBaseDir); }


std::string			Locator::find(const std::string &name) const
{
	std::string		str("");
	StringList::iterator	it;
	for (it = mFolders->begin(); str.empty() && it != mFolders->end(); it++)
	{
		if (*it == name)
			str = *it;
	}
	return (str);
}

shared_logger_ptr	Locator::getLogger() const throw()
{
	return (mLogger);
}

shared_logger_ptr	Locator::logMessage(const String &msg, log_level lvl) throw()
{
	if (mLogger)
		mLogger->insert(msg, lvl);
	return (mLogger);
}

SharedStringList	Locator::getFolders() const
{
	return (mFolders);
}
void				Locator::clearFolders()
{
	mFolders->clear();
}

SharedStringList	Locator::getFiles() const
{
	return (mFiles);
}
void				Locator::clearFiles()
{
	mFiles->clear();
}




ResourceScanHandler::ResourceScanHandler(Locator *loc,
										 const std::string &name,
										 const StringList &interests,
										 const Handler &handler)
	: mName(name)
	, mInterests(interests)
	, mHandler(handler)
	, mLocator(loc)
{}

ResourceScanHandler::ResourceScanHandler(const ResourceScanHandler &rk)
	: mName(rk.mName)
	, mInterests(rk.mInterests)
	, mHandler(rk.mHandler)
	, mLocator(rk.mLocator)
{}
ResourceScanHandler::~ResourceScanHandler()
{}

ResourceScanHandler			&ResourceScanHandler::operator=(const ResourceScanHandler &rk)
{
	mName = rk.mName;
	mInterests = rk.mInterests;
	mHandler = rk.mHandler;
	mLocator = rk.mLocator;
	return (*this);
}

Locator						*ResourceScanHandler::getLocator() throw()
{
	return (mLocator);
}

const Locator				*ResourceScanHandler::getLocator() const throw()
{
	return (mLocator);
}

void						ResourceScanHandler::operator()(Locator *locator,
															SharedStringList strings,
															StringList::iterator iter,
															void *extra)
{
	handle(locator, strings, iter, extra);
}

bool						ResourceScanHandler::shouldHandle(const Path &filepath) const {
	StringList::const_iterator	it;
	bool						found(false);
	for (it = mInterests.begin(); !found && it != mInterests.end(); it++) {
		if (PatternMatch<std::string>(*it, filepath.getBase())) {
			found = true;
		}
	}
	return (found);
}
void						ResourceScanHandler::handle(Locator *locator,
														SharedStringList strings,
														StringList::iterator iter,
														void *extra) {
	if (mHandler)
		mHandler(locator, strings, iter, extra);
}

const std::string			&ResourceScanHandler::getName() const {
	return (mName);
}
const StringList			&ResourceScanHandler::getInterests() const {
	return (mInterests);
}
const ResourceScanHandler::Handler				&ResourceScanHandler::getHandler() const {
	return (mHandler);
}

std::string					&ResourceScanHandler::setName(const std::string &rk) {
	mName = rk;
	return (mName);
}
StringList					&ResourceScanHandler::setInterests(const StringList &rk) {
	mInterests = rk;
	return (mInterests);
}
ResourceScanHandler::Handler	&ResourceScanHandler::setHandler(const Handler &rk) {
	mHandler = rk;
	return (mHandler);
}

using namespace std::placeholders;

StaticResourceHandler::StaticResourceHandler(Locator *loc)
	: ResourceScanHandler(loc, "Static")
{
	mHandler = std::bind(&StaticResourceHandler::work, this, _1, _2, _3, _4);

	mInterests.push_back("*.html");
	mInterests.push_back("*.css");
	mInterests.push_back("*.js");
	mInterests.push_back("*.json");

	mInterests.push_back("*.jpg");
	mInterests.push_back("*.jpeg");
	mInterests.push_back("*.gif");
	mInterests.push_back("*.png");
	mInterests.push_back("*.bmp");
}
StaticResourceHandler::StaticResourceHandler(const StaticResourceHandler &rk)
	: ResourceScanHandler(rk)
{}
StaticResourceHandler::~StaticResourceHandler()
{}

StaticResourceHandler		&StaticResourceHandler::operator=(const StaticResourceHandler &rk) {
	ResourceScanHandler::operator=(rk);
	return (*this);
}
void						StaticResourceHandler::work(Locator *locator,
														SharedStringList strings,
														StringList::iterator iter,
														void *extra)
{
	(void)locator;
	(void)strings;
	(void)extra;
	locator->logMessage("found " + *iter);
}

ACLResourceHandler::ACLResourceHandler(Locator *loc)
	: ResourceScanHandler(loc, "ACL")
{
	mHandler = std::bind(&ACLResourceHandler::work, this, _1, _2, _3, _4);
	mInterests.push_back(".access");
}
ACLResourceHandler::ACLResourceHandler(const ACLResourceHandler &rk)
	: ResourceScanHandler(rk)
{}
ACLResourceHandler::~ACLResourceHandler()
{}

ACLResourceHandler		&ACLResourceHandler::operator=(const ACLResourceHandler &rk) {
	ResourceScanHandler::operator=(rk);
	return (*this);
}

void					ACLResourceHandler::work(Locator *locator,
												 SharedStringList strings,
												 StringList::iterator iter,
												 void *extra)
{
	AccessControlList			acl;
	HTTPServer					*srv(reinterpret_cast<HTTPServer *>(extra));
	std::ifstream				ifs(*iter);

	srv->log("[+] Loading access control file: " + *iter);
	if (!(ifs >> acl)) {
		throw std::runtime_error(*iter + ": failed to parse acl!");
	}
	(void)locator;
	(void)strings;
	srv->getAccessList()->merge(acl);
}

URITranslationHandler::URITranslationHandler(Locator *loc,
											 const URITranslationMap *tr,
											 URITranslationResultMap &res)
	: ResourceScanHandler(loc, "URITranslation")
	, mTranslations(tr)
	, mResults(res)
{
	URITranslationMap::const_iterator it;
	URITranslation::const_iterator jt;
	mHandler = std::bind(&URITranslationHandler::work, this, _1, _2, _3, _4);
	for (it = tr->begin(); it != tr->end(); it++)
	{
		mResults[it->first] = "";
		for (jt = it->second.begin(); jt != it->second.end(); jt++)
		{
			mInterests.push_back(*jt);
		}
	}

}

URITranslationHandler::URITranslationHandler(const URITranslationHandler &rk)
	: ResourceScanHandler(rk)
	, mTranslations(rk.mTranslations)
	, mResults(rk.mResults)
{}
URITranslationHandler::~URITranslationHandler()
{}

URITranslationHandler	&URITranslationHandler::operator=(const URITranslationHandler &rk) {
	ResourceScanHandler::operator=(rk);
	mTranslations = rk.mTranslations;
	mResults = rk.mResults;
	return (*this);
}

void					URITranslationHandler::work(Locator *locator,
												 SharedStringList strings,
												 StringList::iterator iter,
												 void *extra)
{
	URITranslationMap::const_iterator	trIt;
	URITranslation::const_iterator		tryIt;
	URITranslationResultMap::iterator	resIt(mResults.end());
	Path								path(*iter);
	std::string							base(path.getBase());

	for (resIt = mResults.begin(); resIt != mResults.end(); resIt++)
	{
		if (resIt->second == base)
		{
			return ;
		}
	}
	resIt = mResults.end();
	for (trIt = mTranslations->begin(); trIt != mTranslations->end(); trIt++)
	{
		for (tryIt = trIt->second.begin(); tryIt != trIt->second.end(); tryIt++)
		{
			if (*tryIt == base)
			{
				resIt = mResults.find(trIt->first);
				resIt->second = base;
				break ;
			}
		}
	}
	if (resIt != mResults.end())
		locator->logMessage("[+] Found uri translation '" + resIt->second + "' for '" + resIt->first + "'");
	(void)locator;
	(void)strings;
	(void)extra;
}


ETagHandler::ETagHandler(Locator *loc, ETagMap &tr)
	: ResourceScanHandler(loc, "ETag")
	, mTags(tr)
{
	mHandler = std::bind(&ETagHandler::work, this, _1, _2, _3, _4);
	mInterests.push_back("*");
}
ETagHandler::ETagHandler(const ETagHandler &rk)
	: ResourceScanHandler(rk)
	, mTags(rk.mTags)
{}
ETagHandler::~ETagHandler()
{}

ETagHandler	&ETagHandler::operator=(const ETagHandler &rk) {
	ResourceScanHandler::operator=(rk);
	return (*this);
}

void					ETagHandler::work(Locator *locator,
										  SharedStringList strings,
										  StringList::iterator iter,
										  void *extra)
{
	(void)locator;
	(void)extra;
	(void)iter;
	(void)strings;
/*	ETagHash			hasher;
	std::string			path(*iter);
	if (path.find(mLocator->getBaseDir()) != std::string::npos)
		path = path.substr(mLocator->getBaseDir().size());
	mTags[path] = std::to_string(hasher(path));
	if (mLogger)
	(*mLogger) << "tag[" << *iter << "]: " << mTags[*iter] << std::endl;
*/
}
