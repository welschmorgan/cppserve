// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   host.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/22 17:40:05 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/29 16:12:40 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#include "host.hpp"

Host::Host()
	: mName()
	, mPort()
{}
Host::Host(const String &name,
	 const port_type port)
	: mName(name)
	, mPort(port)
{}
Host::Host(const Host &rhs)
	: mName(rhs.mName)
	, mPort(rhs.mPort)
{}
Host::~Host() throw()
{}

Host			&Host::operator=(const Host &h)
{
	mName = h.mName;
	mPort = h.mPort;
	return (*this);
}

Host			&Host::name(const String &name) throw()
{
	mName = name;
	return (*this);
}
Host			&Host::port(const port_type nb) throw()
{
	mPort = nb;
	return (*this);
}

String			Host::name() const throw()
{
	return (mName);
}
Host::port_type	Host::port() const throw()
{
	return (mPort);
}

OStream				&operator<<(OStream &os, const Host &host)
{
	os << host.mName;
	os << ":";
	os << host.mPort;
	return (os);
}
IStream				&operator>>(IStream &is, Host &host)
{
	StringStream	ss;
	size_t			pos;
	String			nm;
	Host::port_type	prt;
	if (!(is >> nm))
		return (is);
	pos = nm.find(":");
	if (pos != std::string::npos)
	{
		ss.str(nm.substr(pos + 1));
		if (!(ss >> prt))
			return (is);
	}
	else if (!(is >> prt))
		return (is);
	host.mName = nm.substr(0, pos);
	host.mPort = prt;
	return (is);
}
