// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   protocol.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/12 16:48:29 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/23 13:53:03 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "protocol.hpp"
#include <sstream>
#include <ostream>
#include <istream>

HTTPProtocol::HTTPProtocol()
	: mMethod()
	, mURI()
	, mVersion()
{}

HTTPProtocol::HTTPProtocol(const String &method,
						   const String &uri,
						   const String &version)
	: mMethod(method)
	, mURI(uri)
	, mVersion(version)
{}

HTTPProtocol::HTTPProtocol(const HTTPProtocol &rk)
	: mMethod(rk.mMethod)
	, mURI(rk.mURI)
	, mVersion(rk.mVersion)
{}

HTTPProtocol::~HTTPProtocol()
{}



StringList			HTTPProtocol::getMethods() {
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
bool				HTTPProtocol::isMethodName(const std::string &name) {
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

void				HTTPProtocol::setVersion(const std::string &s) {
	mVersion = s;
}
void				HTTPProtocol::setURI(const std::string &s) {
	mURI = s;
}
void				HTTPProtocol::setMethod(const std::string &s) {
	mMethod = s;
}

const std::string	&HTTPProtocol::getVersion() const {
	return (mVersion);
}
const std::string	&HTTPProtocol::getURI() const {
	return (mURI);
}
const std::string	&HTTPProtocol::getMethod() const {
	return (mMethod);
}
std::ostream		&HTTPProtocol::stringify(std::ostream &os) const {
	if (os)
		os << mMethod << " " << mURI << " " << mVersion;
	return (os);
}

std::string			HTTPProtocol::stringify() const {
	std::stringstream ss;
	stringify(ss);
	return (ss.str());
}
bool				HTTPProtocol::parse(std::istream &is) {
	if (!(is >> mMethod))
		return (false);
	if (!isMethodName(mMethod))
		return (false);
	if (!(is >> mURI))
		return (false);
	if (!(is >> mVersion))
		return (false);
	return (true);
}

HTTPProtocol		&HTTPProtocol::operator=(const HTTPProtocol &rk) {
	mVersion = rk.mVersion;
	mURI = rk.mURI;
	mMethod = rk.mMethod;
	return (*this);
}

std::istream		&operator>>(std::istream &is, HTTPProtocol &rk) {
	if (!rk.parse(is))
		is.setstate(std::ios::failbit);
	return (is);
}
std::ostream		&operator<<(std::ostream &os, const HTTPProtocol &rk) {
	rk.stringify(os);
	return (os);
}
