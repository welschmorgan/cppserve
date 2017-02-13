// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   cache.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/12 18:22:14 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/12 19:00:25 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "cache.h"
#include <string.h>

CacheData::CacheData()
	: Serializer<CacheData>()
	, mName()
	, mMime()
	, mSize(0)
	, mData(NULL)
{}
CacheData::CacheData(const std::string			&name,
					 const std::string			&mimes,
					 const size_t				size,
					 const unsigned char		*data)
	: Serializer<CacheData>()
	, mName(name)
	, mMime(mimes)
	, mSize(0)
	, mData(NULL)
{
	setData(data, size);
}
CacheData::CacheData(const CacheData &rk)
	: Serializer<CacheData>()
	, mName(rk.mName)
	, mMime(rk.mMime)
	, mSize(0)
	, mData(NULL)
{
	setData(rk.mData, rk.mSize);
}
CacheData::~CacheData()
{
	setData(NULL, 0);
}

CacheData							&CacheData::operator=(const CacheData &rk)
{

	mName = rk.mName;
	mMime = rk.mMime;
	setData(rk.mData, rk.mSize);
	return (*this);
}

std::string							CacheData::getName() const {
	return (mName);
}
std::string							CacheData::getMimeType() const {
	return (mMime);
}
size_t								CacheData::getSize() const {
	return (mSize);
}
unsigned char						*CacheData::getData() const {
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
unsigned char						*CacheData::setData(const unsigned char *data,
														size_t size) {
	if (mData)
		delete []mData;
	mData = NULL;
	if (data && size) {
		mData = new unsigned char [size];
		if (mData) {
			memcpy(mData, data, size);
		}
	}
	return (mData);
}


std::ostream						&operator<<(std::ostream &os,
												CacheData &c);
std::istream						&operator<<(std::istream &is,
												const CacheData &c);
