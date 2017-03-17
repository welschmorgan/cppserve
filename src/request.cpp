// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   request.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/11 02:31:10 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/14 20:13:46 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "request.h"

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
	bool						data = false;
	size_t						lineid = 0;

	if (!(is >> mProto))
		return (false);
	std::getline(is, mExtra);
	while (std::getline(is, line)) {
		while (!line.empty() && (line.at(0) == ' ' || line.at(0) == '\t'))
			line.erase(line.begin());
		std::cout << lineid << ": " << (!data ? "header: " : "body: ") << line << std::endl;
		if ((pos = line.find_first_of(':')) != std::string::npos) {
			key = line.substr(0, pos);
			val = line.substr(pos + 1);
		} else {
			key = line;
			val = "";
		}
		if (line.empty()) {
			data = true;
		}
		if (data) {
			mBody->push_back(val);
		} else {
			(*mHeaders)[key] = val;
		}
		lineid++;
	}
	return (true);
}

SharedStringMap		HTTPRequest::getHeaders() const {
	return (mHeaders);
}
SharedStringList	HTTPRequest::getBody() const {
	return (mBody);
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
