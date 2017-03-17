// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   response.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/11 11:41:12 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/14 20:27:12 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "response.h"
#include <sstream>

HTTPResponse::HTTPResponse(const HTTPProtocol &proto,
						   bool autoClose)
	: mProto(proto)
	, mAutoClose(autoClose)
	, mWritten(false)
	, mStatus(200)
	, mHeaders()
	, mBody()
{}
HTTPResponse::HTTPResponse(const HTTPResponse &rk)
	: mProto(rk.mProto)
	, mAutoClose(rk.mAutoClose)
	, mWritten(rk.mWritten)
	, mStatus(rk.mStatus)
	, mHeaders(rk.mHeaders)
	, mBody(rk.mBody)
{}
HTTPResponse::~HTTPResponse()
{}

HTTPResponse		&HTTPResponse::operator=(const HTTPResponse &rk)
{
	mProto = rk.mProto;
	mAutoClose = rk.mAutoClose;
	mWritten = rk.mWritten;
	mStatus = rk.mStatus;
	mHeaders = rk.mHeaders;
	mBody = rk.mBody;
	return (*this);
}


int					HTTPResponse::getStatus(int code) const
{ return (mStatus); }
const StringMap		&HTTPResponse::getHeaders() const
{ return (mHeaders); }
const HTTPProtocol	&HTTPResponse::getProtocol() const
{ return (mProto); }
const std::string	&HTTPResponse::getBody() const
{ return (mBody); }

void				HTTPResponse::setStatus(int code)
{
	mStatus = code;
}
void				HTTPResponse::setHeader(const std::string &name,
											const std::string &value)
{
	mHeaders[name] = value;
}
void				HTTPResponse::setBody(const std::string &body)
{
	mBody = body;
}

void				HTTPResponse::reset(const HTTPProtocol &proto,
										int status)
{
	mStatus = status;
	if (!proto.stringify().empty())
		mProto = proto;
	mHeaders.clear();
	mBody.clear();
}

int					HTTPResponse::write(SocketStream &strm)
{
	std::string codestr = std::to_string(mStatus) + " OK";
	std::string lastModif = "Wed, 18 Jun 2003 16:05:58 GMT";
	std::string etag = "56d-9989200-1132c580";
	std::string ctype = "text/html";
	std::string date = "Thu, 19 Feb 2009 12:27:04 GMT";
	std::string servName = "Apache/2.2.3";
	std::stringstream ss;
	ss << mProto << " " << codestr << "\n"
	   << "\n"
	   << "Date: " << date << "\n"
	   << "Server: " << servName << "\n"
	   << "Last-Modified: " << lastModif << "\n"
	   << "ETag: \"" << etag << "\"\n"
	   << "Content-Type: " << ctype << "\n"
	   << "Content-Length: 15" << "\n"
	   << "Accept-Ranges: bytes" << "\n"
	   << "Connection: close" << "\n"
	   << "\n"
	   << mBody;
	std::cout << ss.str() << std::endl;
	strm << ss.str();
	return (mStatus);
}
