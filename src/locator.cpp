// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   locator.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/10 21:41:12 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/13 21:28:05 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "locator.h"

# include <sys/types.h>
# include <dirent.h>

Locator::Locator(const std::string &base_dir)
	: mBaseDir(base_dir)
	, mFolders(new StringList())
	, mFiles(new StringList())
	, mErrors(new StringList())
	, mHandlers()
	, mExtra(NULL)
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
SharedStringList	Locator::getErrors() const {
	return (mErrors);
}

void				Locator::discoverFolder(const std::string &folder) {
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
}

void				Locator::onFileAdded(StringList::iterator str) {
	ResourceScanHandlerList::iterator it;
	SharedResourceScanHandler h;
	for (it = mHandlers.begin(); it != mHandlers.end(); it++) {
		h = *it;
		std::cout << h->getName() << " should handle " << Path(*str).getBase() << " ? " << it->get()->shouldHandle(*str) << std::endl;
		if (it->get()->shouldHandle(*str)) {
			it->get()->handle(this, mFiles, str, mExtra);
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




ResourceScanHandler::ResourceScanHandler(const std::string &name,
										 const StringList &interests,
										 const Handler &handler)
	: mName(name)
	, mInterests(interests)
	, mHandler(handler)
{}

ResourceScanHandler::ResourceScanHandler(const ResourceScanHandler &rk)
	: mName(rk.mName)
	, mInterests(rk.mInterests)
	, mHandler(rk.mHandler)
{}
ResourceScanHandler::~ResourceScanHandler()
{}

ResourceScanHandler			&ResourceScanHandler::operator=(const ResourceScanHandler &rk)
{
	mName = rk.mName;
	mInterests = rk.mInterests;
	mHandler = rk.mHandler;
	return (*this);
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


StaticResourceHandler::StaticResourceHandler()
	: ResourceScanHandler("Static")
{
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

ACLResourceHandler::ACLResourceHandler()
	: ResourceScanHandler("ACL")
{
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
