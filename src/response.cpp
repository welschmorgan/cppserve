// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   response.cpp                                       :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/11 11:41:12 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/08 14:31:56 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "response.h"
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>

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
	std::stringstream	ss;
	std::string			endl("\r\n");
	std::string			codestr = std::to_string(mStatus) + " OK";

	ss << mProto.getVersion() << " " << codestr << endl;
	for (StringMap::iterator it = mHeaders.begin(); it != mHeaders.end(); it++)
		ss << it->first << ": " << it->second << endl;
	ss << endl
	   << mBody;
	std::cout << "Response:" << std::endl
			  << std::string(10, '-') << std::endl
			  << ss.str() << std::endl;
	strm << ss.str();
	strm.sync();
	return (mStatus);
}
