// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   socket.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/07 21:03:39 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/29 19:19:45 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "socket.hpp"


const SocketStage			SocketStage::None(0);
const SocketStage			SocketStage::Opened(1);
const SocketStage			SocketStage::Closed(2);
const SocketStage			SocketStage::Bound(3);
const SocketStage			SocketStage::Listening(4);
const SocketStage			SocketStage::Count(0);

SocketStage::SocketStage(Id val)
	: data(val)
{}

SocketStage::SocketStage(const SocketStage &rk)
	: data(rk.data)
{}

SocketStage::~SocketStage()
{}

SocketStage &SocketStage::operator=(const SocketStage &rk) {
	data = rk.data;
	return (*this);
}

std::string					SocketStage::name() const {
	const NameMap			*names(Names());
	NameMap::const_iterator it(names->find(data));
	std::string				ret;
	if (it != names->end())
		ret = it->second;
	return (ret);
}

SocketStage::Id				SocketStage::value() const {
	return (data);
}


bool					SocketStage::operator<(const SocketStage &rk) const {
	return (data < rk.data);
}

bool					SocketStage::operator>(const SocketStage &rk) const {
	return (data > rk.data);
}

bool					SocketStage::operator<=(const SocketStage &rk) const {
	return (data <= rk.data);
}
bool					SocketStage::operator>=(const SocketStage &rk) const {
	return (data >= rk.data);
}

bool					SocketStage::operator==(const SocketStage &rk) const {
	return (data == rk.data);
}

bool					SocketStage::operator!=(const SocketStage &rk) const {
	return (data != rk.data);
}

SocketStage::operator	std::string() const {
	return (name());
}

SocketStage::operator	Id() const {
	return (value());
}


const SocketStage::NameMap	*SocketStage::Names() {
	static const NameMap	names
	{
		{ None.data, "none" },
		{ Opened.data, "opened" },
		{ Closed.data, "closed" },
		{ Bound.data, "bound" },
		{ Listening.data, "listening" },
		{ Count.data, "unknown" },
	};
	return (&names);
}


std::ostream &operator<<(std::ostream &os, const SocketStage &stg) {
	os << stg.value() << ": " << stg.name();
	return (os);
}
