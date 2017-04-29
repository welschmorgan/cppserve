// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   address.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/10 20:36:43 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/10 21:04:15 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "address.hpp"



Address::Address(sockaddr_in *a)
	: mFamily(AF_INET)
	, mAddr()
	, mPort(0)
{
	static char					buf[32] = { 0 };
	if (a) {
		mFamily = a->sin_family;
		inet_ntop(mFamily, (void*)a, &buf[0], 32);
		mAddr = std::string(&buf[0], &buf[0] + 32);
		mPort = a->sin_port;
	}
}

Address::Address(int family, const std::string &addr, uint16_t port)
	: mFamily(family)
	, mAddr(addr)
	, mPort(port)
{}
Address::Address(const Address &rk)
	: mFamily(rk.mFamily)
	, mAddr(rk.mAddr)
	, mPort(rk.mPort)
{}
Address::~Address()
{}

Address				&Address::operator=(const Address &rk)
{
	mFamily = rk.mFamily;
	mAddr = rk.mAddr;
	mPort = rk.mPort;
	return (*this);
}

Address::operator	std::string() const
{
	std::string str("");
	str += mAddr + ":";
	str += std::to_string(mPort);
	return (str);
}

std::ostream		&operator<<(std::ostream &os, const Address &rk) {
	os << (std::string)rk;
	return (os);
}


void				Address::update(int family,
									const std::string &addr,
									uint16_t port)
{
	mFamily = family;
	mAddr = addr;
	mPort = port;
}

int					Address::getFamily() const
{
	return (mFamily);
}
const std::string	&Address::getAddress() const
{
	return (mAddr);
}
uint16_t			Address::getPort() const
{
	return (mPort);
}


bool				Address::operator!=(const Address &rk)
{
	return (!(*this == rk));
}

bool				Address::operator==(const Address &rk)
{
	return (mAddr == rk.mAddr && mPort == rk.mPort);
}
