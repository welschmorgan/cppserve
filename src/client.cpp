// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   client.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/05 13:55:48 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/17 19:13:57 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "client.h"
#include <sstream>

HTTPClient::HTTPClient(SocketStream::ptr strm,
					   const Address &addr)
	: mStream(strm)
	, mAddress(addr)
	, mPID(0)
	, mRequest(new HTTPRequest())
{
}

HTTPClient::~HTTPClient() {
	close();
}


HTTPClient::operator std::string() const {
	return ((std::string)mAddress);
}

void				HTTPClient::parseRequest() {
	mRequest->parse(*mStream);
}
int					HTTPClient::getPID() const {
	return (mPID);
}
void				HTTPClient::_setPID(int pid) {
	mPID = pid;
}

SocketStream::ptr	HTTPClient::getStream() const {
	return (mStream);
}

SharedHTTPRequest	HTTPClient::getRequest() const {
	return (mRequest);
}

bool				HTTPClient::open(const std::string &host, uint16_t port) {
	return (mStream->open(host, port));
}

void				HTTPClient::close() {
	if (mStream) {
		if (mStream->isOpen()) {
			std::cout << "[+] Closing client socket " << *this << std::endl;
		}
		mStream->close();
	}
}


bool				HTTPClient::writeResponse(SharedHTTPResponse res) {
	SocketStream	*strm(mStream.get());
	return (res->write(*mStream));
}
const Address		&HTTPClient::getAddress() const {
	return (mAddress);
}

std::ostream		&operator<<(std::ostream &os, const HTTPClient &c) {
	os << (const std::string)c;
	return (os);
}
