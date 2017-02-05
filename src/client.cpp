// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   client.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/05 13:55:48 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/05 18:50:12 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "client.h"

HTTPClient::HTTPClient(SocketStream::ptr strm,
					   short family,
					   const std::string &addr,
					   uint16_t port)
	: mStream(strm)
	, mFamily(family)
	, mAddress(addr)
	, mPort(port)
	, mPID(0)
{
}

HTTPClient::~HTTPClient() {
	close();
}


HTTPClient::operator std::string() const {
	std::string str("");
	str += (mFamily == AF_INET ? "ipv4" : "ipv6") + std::string(" ");
	str += mAddress + ":";
	str += std::to_string(mPort);
	return (str);
}
int					HTTPClient::getPID() const {
	return (mPID);
}
void				HTTPClient::_setPID(int pid) {
	mPID = pid;
}


short				HTTPClient::getFamily() const {
	return (mFamily);
}

std::string			HTTPClient::getAddress() const {
	return (mAddress);
}

uint16_t			HTTPClient::getPort() const {
	return (mPort);
}

SocketStream::ptr	HTTPClient::getStream() const {
	return (mStream);
}


bool				HTTPClient::open(const std::string &host, uint16_t port) {
	return (mStream->open(host, port));
}

void				HTTPClient::close() {
	mStream->close();
}

std::ostream		&operator<<(std::ostream &os, const HTTPClient &c) {
	os << (const std::string)c;
	return (os);
}
