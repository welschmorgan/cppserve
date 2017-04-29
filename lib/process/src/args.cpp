// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   logger.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/15 14:17:16 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/23 18:13:20 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "libprocess.hpp"
#include <iostream>

namespace proc
{
	arg_list::arg_list(int argc, char *const argv[])
		: m_args()
		, m_regs()
	{
		int i = 0;
		for (i = 0; i < argc; i++) {
			m_args.push_back(arg(argv[i]));
		}
	}

	arg_list::arg_list(const arg_list &rk)
		: m_args(rk.m_args)
		, m_regs(rk.m_regs)
	{}

	arg_list::~arg_list()
	{}

	arg_list		&arg_list::operator=(const arg_list &rk) {
		m_args = rk.m_args;
		m_regs = rk.m_regs;
		return (*this);
	}

	const arg		*arg_list::get(const std::string &name) const {
		container::const_iterator it;
		std::vector<std::string>::const_iterator jt;
		const std::vector<std::string> *triggers;
		for (it = m_regs.begin(); it != m_regs.end(); it++) {
			if (it->name() == name) {
				return (&(*it));
			}
			triggers = &it->triggers();
			for (jt = triggers->begin(); jt != triggers->end(); jt++) {
				if ((*jt) == name)
					return (&(*it));
			}
		}
		return (NULL);
	}

	arg			*arg_list::get(const std::string &name) {
		container::iterator it;
		std::vector<std::string>::const_iterator jt;
		const std::vector<std::string> *triggers;
		for (it = m_regs.begin(); it != m_regs.end(); it++) {
			if (it->name() == name) {
				return (&(*it));
			}
			triggers = &it->triggers();
			for (jt = triggers->begin(); jt != triggers->end(); jt++) {
				if (*jt == name)
					return (&(*it));
			}
		}
		return (NULL);
	}

	bool					arg_list::set(const std::string &name,
										  const std::vector<std::string> &triggers,
										  arg::callback handler,
										  const std::string &value,
										  const std::string &desc) {
		arg	*opt;

		opt = get(name);
		if (opt) {
			opt->triggers(triggers);
			opt->desc(desc);
			opt->value(value);
			opt->handler(handler);
		} else {
			m_regs.push_back(arg(name, triggers, handler, value));
		}
		return (false);
	}

	bool					arg_list::contains(const std::string &name) const {
		container::const_iterator it;
		std::vector<std::string>::const_iterator jt;
		const std::vector<std::string> *triggers;
		for (it = m_regs.begin(); it != m_regs.end(); it++) {
			if (it->name() == name) {
				return (true);
			}
			triggers = &it->triggers();
			for (jt = triggers->begin(); jt != triggers->end(); jt++) {
				if (*jt == name) {
					return (true);
				}
			}
		}
		return (false);
	}

	void				arg_list::parse(void *data) {
		container::iterator it;
		arg::callback hdr;
		arg *opt;
		for (it = m_args.begin() + 1; it != m_args.end(); it++)
		{
			opt = get(it->name());
			if (opt) {
				opt->value(it->value());
				opt->active(true);
				hdr = it->handler();
				if (hdr) {
					hdr(data);
				}
				hdr = opt->handler();
				if (hdr) {
					hdr(data);
				}
			} else {
				throw std::runtime_error("unkown option: " + it->name());
			}
		}
	}

	arg_list::container			*arg_list::supplied() throw()
	{
		return (&m_args);
	}
	const arg_list::container	*arg_list::supplied() const throw()
	{
		return (&m_args);
	}
	arg_list::container			*arg_list::registered() throw()
	{
		return (&m_regs);
	}
	const arg_list::container	*arg_list::registered() const throw()
	{
		return (&m_regs);
	}


	arg::arg(const std::string &name,
			 const std::vector<std::string> &triggers,
			 callback handler,
			 const std::string &value,
			 const std::string &desc)
		: m_name(name)
		, m_triggers(triggers)
		, m_handler(handler)
		, m_value(value)
		, m_desc(desc)
		, m_active(false)
	{
		size_t pos(name.find('='));
		if (pos != std::string::npos) {
			m_name = name.substr(0, pos);
			m_value = name.substr(pos + 1);
		}
	}

	arg::arg(const arg &rk)
		: m_name(rk.m_name)
		, m_triggers(rk.m_triggers)
		, m_handler(rk.m_handler)
		, m_value(rk.m_value)
		, m_desc(rk.m_desc)
		, m_active(rk.m_active)
	{}

	arg::~arg()
	{}

	arg &arg::operator=(const arg &rk) {
		m_name = rk.m_name;
		m_triggers = rk.m_triggers;
		m_handler = rk.m_handler;
		m_value = rk.m_value;
		m_desc = rk.m_desc;
		m_active = rk.m_active;
		return (*this);
	}


	const std::string	&arg::name() const throw() { return (m_name); }
	const std::string	&arg::value() const throw(){ return (m_value); }
	const std::vector<std::string>	&arg::triggers() const throw(){ return (m_triggers); }
	arg::callback		arg::handler() const throw() { return (m_handler); }

	void				arg::NoOp(void *) {}

	void				arg::triggers(const std::vector<std::string> &n) { m_triggers = n; }
	void				arg::handler(callback v) { m_handler = v; }
	void				arg::name(const std::string &n) { m_name = n; }
	void				arg::value(const std::string &v) { m_value = v; }

	void				arg::active(bool state) { m_active = state; }
	bool				arg::active() const throw() { return (m_active); }

	const std::string	&arg::desc() const throw()
	{
		return (m_desc);
	}
	void				arg::desc(const std::string &d)
	{
		m_desc = d;
	}

};
