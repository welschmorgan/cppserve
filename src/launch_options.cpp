// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   launch_options.cpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/04 14:17:26 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/05 13:22:59 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "launch_options.h"
#include <iostream>

LaunchOptions::LaunchOptions(int argc, char *const argv[])
	: mArgs()
	, mRegs()
{
	int i = 0;
	for (i = 0; i < argc; i++) {
		mArgs.push_back(LaunchOption(argv[i]));
	}
}

LaunchOptions::LaunchOptions(const LaunchOptions &rk)
	: mArgs(rk.mArgs)
	, mRegs(rk.mRegs)
{}

LaunchOptions::~LaunchOptions()
{}

LaunchOptions			&LaunchOptions::operator=(const LaunchOptions &rk) {
	mArgs = rk.mArgs;
	mRegs = rk.mRegs;
	return (*this);
}

const LaunchOption		*LaunchOptions::get(const std::string &name) const {
	ArgList::const_iterator it;
	std::vector<std::string>::const_iterator jt;
	const std::vector<std::string> *triggers;
	for (it = mRegs.begin(); it != mRegs.end(); it++) {
		if (it->getName() == name) {
			return (&(*it));
		}
		triggers = &it->getTriggers();
		for (jt = triggers->begin(); jt != triggers->end(); jt++) {
			if (*jt == name || (std::string("-") + *jt) == name) {
				return (&(*it));
			}
		}
	}
	return (NULL);
}

LaunchOption			*LaunchOptions::get(const std::string &name) {
	ArgList::iterator it;
	std::vector<std::string>::const_iterator jt;
	const std::vector<std::string> *triggers;
	for (it = mRegs.begin(); it != mRegs.end(); it++) {
		if (it->getName() == name) {
			return (&(*it));
		}
		triggers = &it->getTriggers();
		for (jt = triggers->begin(); jt != triggers->end(); jt++) {
			if (*jt == name || (std::string("-") + *jt) == name) {
				return (&(*it));
			}
		}
	}
	return (NULL);
}

bool					LaunchOptions::set(const std::string &name,
										   const std::vector<std::string> &triggers,
										   LaunchOption::Handler handler,
										   const std::string &value) {
	LaunchOption	*opt;

	opt = get(name);
	if (opt) {
		opt->setTriggers(triggers);
		opt->setValue(value);
		opt->setHandler(handler);
	} else {
		mRegs.push_back(LaunchOption(name, triggers, handler, value));
	}
	return (false);
}

bool					LaunchOptions::contains(const std::string &name) const {
	ArgList::const_iterator it;
	std::vector<std::string>::const_iterator jt;
	const std::vector<std::string> *triggers;
	for (it = mRegs.begin(); it != mRegs.end(); it++) {
		if (it->getName() == name) {
			return (true);
		}
		triggers = &it->getTriggers();
		for (jt = triggers->begin(); jt != triggers->end(); jt++) {
			if (*jt == name) {
				return (true);
			}
		}
	}
	return (false);
}

void				LaunchOptions::parse() {
	ArgList::iterator it;
	LaunchOption::Handler hdr;
	LaunchOption *opt;
	for (it = mArgs.begin(); it != mArgs.end(); it++)
	{
		opt = get(it->getName());
		if (opt) {
			//std::cout << "[+] Found: " << opt->getName() << std::endl;
			opt->setValue(it->getValue());
			opt->setActive(true);
			hdr = it->getHandler();
			if (hdr) {
				hdr();
			}
		}
	}
}


LaunchOption::LaunchOption(const std::string &name,
						   const std::vector<std::string> &triggers,
						   Handler handler,
						   const std::string &value)
	: mName(name)
	, mTriggers(triggers)
	, mHandler(handler)
	, mValue(value)
	, mActive(false)
{
	size_t pos(name.find('='));
	if (pos != std::string::npos) {
		mName = name.substr(0, pos);
		mValue = name.substr(pos + 1);
	}
}

LaunchOption::LaunchOption(const LaunchOption &rk)
	: mName(rk.mName)
	, mValue(rk.mValue)
	, mHandler(rk.mHandler)
	, mTriggers(rk.mTriggers)
	, mActive(rk.mActive)
{}

LaunchOption::~LaunchOption()
{}

LaunchOption &LaunchOption::operator=(const LaunchOption &rk) {
	mName = rk.mName;
	mValue = rk.mValue;
	mTriggers = rk.mTriggers;
	mHandler = rk.mHandler;
	mActive = rk.mActive;
	return (*this);
}


const std::string	&LaunchOption::getName() const { return (mName); }
const std::string	&LaunchOption::getValue() const { return (mValue); }
const std::vector<std::string>	&LaunchOption::getTriggers() const { return (mTriggers); }
LaunchOption::Handler	LaunchOption::getHandler() const { return (mHandler); }

void				LaunchOption::NoOp(void) {}

void				LaunchOption::setTriggers(const std::vector<std::string> &n) { mTriggers = n; }
void				LaunchOption::setHandler(Handler v) { mHandler = v; }
void				LaunchOption::setName(const std::string &n) { mName = n; }
void				LaunchOption::setValue(const std::string &v) { mValue = v; }

void				LaunchOption::setActive(bool state) { mActive = state; }
bool				LaunchOption::isActive() const { return (mActive); }
