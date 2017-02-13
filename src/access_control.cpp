// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   access_control.cpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/10 23:30:26 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/11 01:27:54 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

# include "access_control.h"

AccessControlSection::AccessControlSection(const std::string &name,
										   Mode m)
	: mName(name)
	, mHeaders()
	, mIPs()
	, mURIs()
	, mMode(m)
{}

void							AccessControlSection::setMode(Mode m) {
	mMode = m;
}


const std::string				&AccessControlSection::getName() const {
	return (mName);

}
AccessControlSection			&AccessControlSection::addHeader(const std::string &key,
																 const std::string &val)
{
	mHeaders[key] = val;
	return (*this);
}
AccessControlSection			&AccessControlSection::addIP(const std::string &str)
{
	mIPs.push_back(str);
	return (*this);
}
AccessControlSection			&AccessControlSection::addURI(const std::string &str)
{
	mURIs.push_back(str);
	return (*this);
}

AccessControlSection			&AccessControlSection::operator+=(const AccessControlSection &rk) {
	if (mMode != rk.mMode) {
		throw std::runtime_error("Cannot add two sections of different modes!");
	}
	for (StringMap::const_iterator it = rk.mHeaders.begin(); it != rk.mHeaders.end(); it++) {
		mHeaders[it->first] = it->second;
	}
	for (StringList::const_iterator it = rk.mURIs.begin(); it != rk.mURIs.end(); it++) {
		mURIs.push_back(*it);
	}
	for (StringList::const_iterator it = rk.mIPs.begin(); it != rk.mIPs.end(); it++) {
		mIPs.push_back(*it);
	}
	return (*this);
}

const StringMap				&AccessControlSection::getHeaders() const
{ return (mHeaders); }
const StringList			&AccessControlSection::getIPs() const
{ return (mIPs); }
const StringList			&AccessControlSection::getURIs() const
{ return (mURIs); }

AccessControlMode			AccessControlSection::getMode() const
{ return (mMode); }

StringMap					&AccessControlSection::getHeaders()
{ return (mHeaders); }
StringList					&AccessControlSection::getIPs()
{ return (mIPs); }
StringList					&AccessControlSection::getURIs()
{ return (mURIs); }


AccessControlSection::AccessControlSection(const AccessControlSection &rk)
	: mName(rk.mName)
	, mHeaders(rk.mHeaders)
	, mIPs(rk.mIPs)
	, mURIs(rk.mURIs)
	, mMode(rk.mMode)
{}
AccessControlSection::~AccessControlSection()
{}

AccessControlSection			&AccessControlSection::operator=(const AccessControlSection &rk)
{
	mName = rk.mName;
	mHeaders = rk.mHeaders;
	mIPs = rk.mIPs;
	mURIs = rk.mURIs;
	mMode = rk.mMode;
	return (*this);
}

AccessControlList::AccessControlList()
	: mSections(new SectionList())
	, mSection()
{}
AccessControlList::AccessControlList(const AccessControlList &rk)
	: mSections(rk.mSections)
	, mSection(rk.mSection)
{}
AccessControlList::~AccessControlList()
{}

void									AccessControlList::merge(const AccessControlList &rk) {
	SectionList::const_iterator jt;
	SharedSectionList sections(rk.getSections());
	SharedSection	sec;
	for (jt = sections->begin(); jt != sections->end(); jt++) {
		sec = addSection(jt->second->getName(), jt->second->getMode());
		*sec += *jt->second;
	}
}

AccessControlList						&AccessControlList::operator=(const AccessControlList &rk)
{
	mSections = rk.mSections;
	mSection = rk.mSection;
	return (*this);
}

AccessControlList::SharedSection		AccessControlList::getSection(const std::string &name)
{
	SectionList::iterator it(mSections->find(name));
	if (it != mSections->end()) {
		return (it->second);
	}
	return (NULL);
}
const AccessControlList::SharedSection	AccessControlList::getSection(const std::string &name) const {
	SectionList::const_iterator it(mSections->find(name));
	if (it != mSections->end()) {
		return (it->second);
	}
	return (NULL);
}

AccessControlList::SharedSection		AccessControlList::getCurrentSection() {
	return (mSection);
}
const AccessControlList::SharedSection	AccessControlList::getCurrentSection() const {
	return (mSection);
}

const AccessControlList::SharedSectionList	AccessControlList::getSections() const {
	return (mSections);
}
AccessControlList::SharedSection		AccessControlList::addSection(const std::string &name,
																	  Mode mode) {
	SectionList::iterator it(mSections->find(name));
	if (it == mSections->end()) {
		(*mSections)[name] = SharedSection(new Section(name, mode));
		it = mSections->find(name);
	}
	mSection = it->second;
	return (it->second);
}
void									AccessControlList::clearSections() {
	mSections->clear();
}
#include <iostream>
# define PARSE_ERROR(msg) 	throw std::runtime_error("l" + std::to_string(lineid) + ": " + std::string(msg));
void						AccessControlList::parse(std::istream &is)
{
	std::string line, mode, key, val;
	std::stringstream sstrm;
	Mode rmode;
	unsigned lineid = 0;
	while (is.good() && std::getline(is, line))
	{
		while (!line.empty() && (line.at(0) == ' ' || line.at(0) == '\t'))
			line.erase(line.begin());
		if (!line.empty() && line.at(0) == '#')
			continue;
		sstrm.clear();
		sstrm.str(line);
		if (!(sstrm >> mode))
			continue;
		if (mode == "section") {
			if (!(sstrm >> mode))
				PARSE_ERROR("EOF reached, expected keyword");
			if (mode == "include") {
				rmode = ACM_INCLUDE;
			} else if (mode == "exclude") {
				rmode = ACM_EXCLUDE;
			} else {
				PARSE_ERROR("Unknown mode '" + mode + "'");
			}
			if (!(sstrm >> key))
				PARSE_ERROR("EOF reached, expected \"include|exclude\"");
			std::cout << "* add section: " << key << std::endl;
			this->addSection(key, rmode);
		} else if (mode == "header") {
			if (!(sstrm >> key))
				PARSE_ERROR("Missing header name!");
			if (!(sstrm >> val))
				PARSE_ERROR("Missing header value!");
			std::cout << "* add header: " << key << val << std::endl;
			mSection->addHeader(key, val);
		} else if (mode == "ip") {
			if (!(sstrm >> val))
				return ;
			std::cout << "* add ip: " << val << std::endl;
			mSection->addIP(val);
		} else if (mode == "uri") {
			if (!(sstrm >> val))
				return ;
			std::cout << "* add uri: " << val << std::endl;
			mSection->addURI(val);
		}
		lineid++;
	}
}

# undef PARSE_ERROR

void						AccessControlList::serialize(std::ostream &os) const
{}

std::istream				&operator >>(std::istream &is, AccessControlList &acl)
{
	acl.parse(is);
	return (is);
}
std::ostream				&operator <<(std::ostream &os, const AccessControlList &acl)
{
	acl.serialize(os);
	return (os);
}
