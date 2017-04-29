/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 17:03:03 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/17 19:13:33 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		BUFFER_H
# define	BUFFER_H

# include "forward.hpp"

# include <iostream>
# include <sstream>
# include <ctime>
# include <cassert>

template<typename stream_t,
		 typename level_t>
class										basic_logger_buf
	: public std::basic_stringbuf<typename stream_t::char_type>
{
public:
	typedef std::basic_stringbuf<typename stream_t::char_type>		self_type;
	typedef stream_t												stream_type;
	typedef typename stream_type::char_type							char_type;
	typedef std::basic_stringbuf<char_type>							buf_type;
	typedef std::basic_string<char_type>							string_type;
	typedef unsigned												flag_type;
	typedef level_t													level_type;
	typedef basic_logger<stream_t, level_t>							logger_type;
	typedef log_event_bus<stream_t, level_t>						bus_type;
	typedef log_event_bus_ptr<stream_t, level_t>					bus_ptr;

public:
	static const level_t									default_level = level_t(LL_NORMAL);
	static const flag_type									default_flags = flag_type(LF_TIMESTAMP);

	basic_logger_buf(logger_type *parent,
					 stream_type *strm,
					 bus_ptr bus,
					 const string_type &msg = string_type(),
					 level_type lvl = default_level,
					 const flag_type flags = flag_type())
		: buf_type(msg)
		, m_parent(parent)
		, m_date()
		, m_bus(bus)
		, m_flags(flags)
		, m_level(LL_NORMAL)
		, m_threshold(lvl)
		, m_stream(strm)
		{
			time(&m_date);
		}
	basic_logger_buf(const basic_logger_buf &) = delete;
	virtual ~basic_logger_buf() throw()
		{}

	basic_logger_buf		&operator=(const basic_logger_buf &) = delete;

	const stream_type		*stream() const throw() { return (m_stream); }
	stream_type				*stream() throw() { return (m_stream); }

	const logger_type		*parent() const throw() { return (m_parent); }
	logger_type				*parent() throw() { return (m_parent); }

	const bus_ptr			bus() const throw() { return (m_bus); }
	bus_ptr					bus() throw() { return (m_bus); }

	void					stream(stream_type *s) throw() { m_stream = s; }
	void					bus(const bus_ptr &b) throw() {
		assert(b && "Invalid bus for log buffer!");
		b->parent(m_parent);
		m_bus = b;
	}

	const time_t			&date() const throw() { return (m_date); }

	const string_type		&message() const throw() { return (buf_type::str()); }

	flag_type				flags() const throw() { return (m_flags); }
	basic_logger_buf		&flags(const flag_type lvl) throw() { m_flags = lvl; return (*this); }

	level_type				level() const throw()
		{ return (m_level); }
	basic_logger_buf		&level(const level_type lvl) throw()
		{
			m_level = lvl;
			return (*this);
		}

	level_type				threshold() const throw()
		{ return (m_threshold); }
	basic_logger_buf		&threshold(const level_type lvl) throw()
		{
			m_threshold = lvl;
			return (*this);
		}


	static string_type		timestamp(time_t at) throw()
		{
			static char_type	buffer[80] = { 0 };
			struct tm			*ti;

			ti = localtime (&at);
			strftime (buffer, 80, "%T", ti);
			return (string_type(buffer));
		}
	basic_logger_buf		&flush()
		{
			m_level = LL_NORMAL;
			buf_type::str(string_type());
			m_stream->flush();
			return (*this);
		}

	int						sync()
		{
			if (m_level <= m_threshold && m_bus && !buf_type::str().empty())
			{

				if (!m_bus->pre_sync())
					return (0);
				m_bus->format_entry();
				m_bus->post_sync();
			}
			flush();
			return (0);
		}

protected:
	logger_type				*m_parent;
	time_t					m_date;
	bus_ptr					m_bus;
	flag_type				m_flags;
	level_type				m_level;
	level_type				m_threshold;
	stream_type				*m_stream;
};

#endif
