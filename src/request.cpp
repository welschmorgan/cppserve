// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   request.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/11 02:31:10 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/22 16:49:32 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <sstream>
#include <algorithm>
#include "request.hpp"
#include "string.hpp"

HTTPRequest::HTTPRequest(SharedStringMap	headers,
						 SharedStringList	body,
						 const HTTPProtocol	&proto,
						 const std::string	&extra)
	: mHeaders(headers)
	, mBody(body)
	, mProto(proto)
	, mExtra(extra)
{}
HTTPRequest::HTTPRequest()
	: mHeaders(new StringMap())
	, mBody(new StringList())
	, mProto()
	, mExtra()
{}
HTTPRequest::HTTPRequest(const HTTPRequest &rk)
	: mHeaders(rk.mHeaders)
	, mBody(rk.mBody)
	, mProto(rk.mProto)
	, mExtra(rk.mExtra)
{}
HTTPRequest::~HTTPRequest()
{}

HTTPRequest		&HTTPRequest::operator=(const HTTPRequest &rk) {
	mHeaders = rk.mHeaders;
	mBody = rk.mBody;
	mProto = rk.mProto;
	mExtra = rk.mExtra;
	return (*this);
}

bool			HTTPRequest::parse(SocketStream &is) {
	std::string					line, key, val;
	size_t						pos;
	bool						headers = false;
	bool						data = false;
	size_t						lineid = 0;

	if (!(is >> mProto))
		return (false);
	std::cout << lineid << ": " << mProto << std::endl;
	while (is.good()) {
		std::getline(is, line);
		while (!line.empty() && (line.at(0) == ' ' || line.at(0) == '\t'))
			line.erase(line.begin());
		while (!line.empty() && line.at(line.size() - 1) == '\n')
			line.erase(line.begin() + line.size() - 1);
		if (line.empty()
# ifdef UNICODE
			|| !wcsncmp(line.c_str(), "\r", 2))
# else
			|| !strncmp(line.c_str(), "\r", 2))
# endif
		{
			if (!headers)
			{
				std::cout << "header mode" << std::endl;
				headers = true;
			}
			else if (!data)
			{
				if (mProto.getMethod() != "GET")
				{
					std::cout << "body mode" << std::endl;
					data = true;
				}
				else
				{
					break ;
				}
			}
			else
			{
				break ;
			}
		}
		else
		{
			std::cout << lineid << ": " << (!data ? "header: " : "body: ") << line << std::endl;
			if ((pos = line.find_first_of(':')) != std::string::npos)
			{
				key = line.substr(0, pos);
				val = line.substr(pos + 1);
			}
			else
			{
				key = line;
				val = "";
			}
			if (data)
				mBody->push_back(val);
			else
				(*mHeaders)[key] = val;
		}
		lineid++;
	}
	std::cout << lineid << ": " << "done reading client request!" << std::endl;
	return (true);
}

String				HTTPRequest::getHeader(const String &k) const
{
	StringMap::const_iterator	it(mHeaders->find(k));
	String						ret;
	if (it != mHeaders->end())
		ret = it->second;
	return (ret);
}

HTTPRequest			&HTTPRequest::setHeader(const String &k,
											const String &v)
{
	(*mHeaders)[k] = v;
	return (*this);
}

size_t				HTTPRequest::write(SocketStream &os) {
	size_t			ret(0);
	std::stringstream	ss;
	ss << mProto;
	for (auto line : *mHeaders) {
		ss << line.first << ": " << line.second << std::endl;
	}
	for (auto line : *mBody) {
		ss << line << std::endl;
	}
	ret = ss.str().size();
	os << ss.str();
	return (ret);
}

SharedStringMap		HTTPRequest::getHeaders() const {
	return (mHeaders);
}
SharedStringList	HTTPRequest::getBody() const {
	return (mBody);
}

std::string			HTTPRequest::getHeader(const std::string name,
										   bool *found) const throw()
{
	*found = false;
	std::string ret;
	StringMap::const_iterator it;
	std::string lname(name), lhname;
	std::transform(lname.begin(), lname.end(), lname.begin(), ::tolower);
	for (it = mHeaders->begin(); it != mHeaders->end(); it++)
	{
		lhname = it->first;
		std::transform(lhname.begin(), lhname.end(), lhname.begin(), ::tolower);
		if (lname == lhname)
			break ;
	}
	if (it != mHeaders->end())
	{
		ret = it->second;
		*found = true;
	}
	return (ret);
}

bool				HTTPRequest::hasHeader(const std::string name) const throw()
{
	bool			found(false);
	getHeader(name, &found);
	return (found);
}


std::string			HTTPRequest::getURI() const {
	return (mProto.getURI());
}
std::string			HTTPRequest::getVersion() const {
	return (mProto.getVersion());
}
std::string			HTTPRequest::getMethod() const {
	return (mProto.getMethod());
}
std::string			HTTPRequest::getExtra() const {
	return (mExtra);
}
HTTPProtocol		HTTPRequest::getProtocol() const {
	return (mProto);
}


HTTPProtocol		&HTTPRequest::setProtocol(const HTTPProtocol &p) {
	return (mProto = p);
}

HTTPProtocol		&HTTPRequest::setProtocol(const String &method,
											  const String &uri,
											  const String &version)
{
	return (mProto = HTTPProtocol(method, uri, version));
}

SharedStringMap		HTTPRequest::setHeaders(const SharedStringMap &m) {
	return (mHeaders = m);
}
SharedStringList	HTTPRequest::setBody(const SharedStringList &m) {
	return (mBody = m);
}
std::string			HTTPRequest::setURI(const std::string &uri) {
	mProto.setURI(uri);
	return (mProto.getURI());
}
std::string			HTTPRequest::setVersion(const std::string &proto) {
	mProto.setVersion(proto);
	return (mProto.getVersion());
}
std::string			HTTPRequest::setMethod(const std::string &meth) {
	mProto.setMethod(meth);
	return (mProto.getMethod());
}
std::string			HTTPRequest::setExtra(const std::string &extra) {
	return (mExtra = extra);
}
