// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   socket.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/07 21:03:39 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/07 21:37:03 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "socket.h"


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

std::string				SocketStage::name() const {
	NameMap::const_iterator it(Names().find(data));
	std::string				ret;
	if (it != Names().end()) {
		ret = it->second;
	}
	return (ret);
}

SocketStage::Id			SocketStage::value() const {
	return (data);
}


bool					SocketStage::operator==(const SocketStage &rk) const {
	return (data == rk.data);
}
bool					SocketStage::operator==(Id id) const {
	return (data == id);
}

bool					SocketStage::operator!=(const SocketStage &rk) const {
	return (!(*this == rk));
}
bool					SocketStage::operator!=(Id id) const {
	return (!(*this == id));
}

SocketStage::operator	std::string() const {
	return (name());
}

SocketStage::operator	Id() const {
	return (value());
}


const SocketStage::NameMap	SocketStage::Names() {
	static NameMap	names;
	if (names.empty()) {
		names[None.data] = "none";
		names[Opened.data] = "opened";
		names[Closed.data] = "closed";
		names[Bound.data] = "bound";
		names[Listening.data] = "listening";
		names[Count.data] = "unknown";
	}
	return (names);
}


std::ostream &operator<<(std::ostream &os, const SocketStage &stg) {
	os << stg.name();
	return (os);
}
