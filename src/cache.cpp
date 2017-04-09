// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   cache.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/12 18:22:14 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/09 20:12:36 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "cache.h"
#include <string.h>

CacheData::CacheData()
	: Serializer<CacheData>()
	, mName()
	, mMime()
	, mData(NULL)
	, mModTime()
	, mLoaded(false)
{}
CacheData::~CacheData()
{
}

bool								CacheData::isLoaded() const throw()
{ return (mLoaded); }

time_t								CacheData::getCreationTime() const throw()
{
	return (mCreateTime);
}

time_t								CacheData::getModificationTime() const throw()
{
	return (mModTime);
}

time_t								CacheData::getAccessTime() const throw()
{
	return (mAccTime);
}

bool								CacheData::load(const std::string &path)
{
	if (mLoaded)
		return (true);
	mName = path;
	std::ifstream ifs(mName, std::ios::in);
	if (!ifs)
		return (false);
	std::ostream os(&mData);
	std::string line;
	while (std::getline(ifs, line))
		os << line << std::endl;
	mLoaded = true;
	time(&mCreateTime);
	time(&mModTime);
	time(&mAccTime);
	return (mLoaded);
}

void								CacheData::setAccessTime(time_t t) throw()
{
	mAccTime = t;
}
void								CacheData::setCreationTime(time_t t) throw()
{
	mCreateTime = t;
}
void								CacheData::setModificationTime(time_t t) throw()
{
	mModTime = t;
}


const std::string					&CacheData::getName() const throw() {
	return (mName);
}
const std::string					&CacheData::getMimeType() const throw() {
	return (mMime);
}
size_t								CacheData::getSize() const throw() {
	return (mData.str().size());
}
const std::stringbuf				&CacheData::getData() const throw() {
	return (mData);
}

std::string							&CacheData::setName(const std::string &rk) {
	mName = rk;
	return (mName);
}
std::string							&CacheData::setMimeType(const std::string &rk) {
	mMime = rk;
	return (mMime);
}
