/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_bus.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 17:02:30 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/23 14:38:53 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		EVENT_BUS_H
# define	EVENT_BUS_H

# include "forward.hpp"

template<typename stream_t,
		 typename level_t>
class											log_event_bus
{
public:
	typedef basic_logger_buf<stream_t, level_t>	buf_type;
	typedef typename buf_type::char_type		char_type;
	typedef typename buf_type::string_type		string_type;
	typedef typename buf_type::level_type		level_type;
	typedef typename buf_type::flag_type		flag_type;
	typedef basic_logger<stream_t, level_t>		logger_type;

private:
	log_event_bus(const log_event_bus &) = delete;
	log_event_bus &operator=(const log_event_bus &) = delete;

protected:
	logger_type									*m_parent;

public:
	log_event_bus(logger_type *parent = NULL) : m_parent(parent) {}
	virtual ~log_event_bus() throw() {}

	logger_type									*parent() { return (m_parent); }
	const logger_type							*parent() const throw() { return (m_parent); }
	void										parent(logger_type *p) { m_parent = p; }

	virtual bool								pre_sync() = 0;
	virtual void								post_sync() = 0;
	virtual void								format_entry(std::ostream *dup = NULL) = 0;
};

template<typename stream_t,
		 typename level_t>
class											default_log_event_bus
	: public log_event_bus<stream_t, level_t>
{
	default_log_event_bus(const default_log_event_bus &) = delete;
	default_log_event_bus &operator=(const default_log_event_bus &) = delete;
public:
	typedef log_event_bus<stream_t, level_t>	base_type;
	typedef typename base_type::buf_type		buf_type;
	typedef typename base_type::char_type		char_type;
	typedef typename base_type::string_type		string_type;
	typedef typename base_type::level_type		level_type;
	typedef typename base_type::flag_type		flag_type;
	typedef typename base_type::logger_type		logger_type;

	default_log_event_bus(logger_type *parent = NULL)
		: base_type(parent) {}
	virtual ~default_log_event_bus() throw() {}

	bool										pre_sync() { return (true); }
	void										post_sync() {}
	void										format_entry(std::ostream *dup = NULL) {
		buf_type								&buf(base_type::m_parent->buffer());
		stream_t								&stream(*buf.stream());
		string_type								content(buf.str());
		flag_type								flags(buf.flags());
		level_type								level(buf.level());
		level_type								threshold(buf.threshold());
		time_t									date(buf.date());
		if ((flags & LF_TIMESTAMP) != LF_NONE)
		{
			stream << "[" << string_type(buf_type::timestamp(date)) << "] ";
			if (dup)
				(*dup) << "[" << string_type(buf_type::timestamp(date)) << "] ";
		}
		if ((flags & LF_LEVEL) != LF_NONE)
		{
			stream << level << "/" << threshold << " | ";
			if (dup)
				(*dup) << level << "/" << threshold << " | ";
		}
		stream << content;
		if (dup)
			(*dup) << content;
	}
};


#endif
