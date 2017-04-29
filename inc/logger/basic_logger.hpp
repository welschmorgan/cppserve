/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_logger.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 17:04:21 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/23 14:42:18 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		BASIC_LOGGER_H
# define	BASIC_LOGGER_H

# include "basic_logger_buf.hpp"
# include "event_bus.hpp"

template<typename stream_t,
		 typename level_t>
class basic_logger
	: public stream_t
{
	basic_logger(const basic_logger &r) = delete;
	basic_logger &operator=(const basic_logger &r) = delete;

public:
	typedef basic_logger<stream_t, level_t>					self_type;
	typedef stream_t										stream_type;
	typedef typename stream_type::char_type					char_type;
	typedef basic_logger_buf<stream_type>					buf_type;
	typedef typename buf_type::bus_type						bus_type;
	typedef typename buf_type::bus_ptr						bus_ptr;
	typedef typename buf_type::flag_type					flag_type;
	typedef level_t											level_type;
	typedef std::basic_string<char_type>					string_type;
	typedef stream_type										&(*manip_type)(stream_type &);

public:
	static const level_t									default_level = buf_type::default_level;
	static const flag_type									default_flags = buf_type::default_flags;

public:
	basic_logger(stream_type *os,
				 const level_type lvl = default_level,
				 const flag_type flags = default_flags)
		: stream_type(&m_buffer)
		, m_stream(os)
		, m_buffer(this,
				   os,
				   bus_ptr(new default_log_event_bus<stream_t, level_t>(this)),
				   string_type(),
				   lvl,
				   flags)
		, m_threshold(lvl)
		{
		}

	basic_logger()
		: stream_type(&m_buffer)
		, m_stream(&std::clog)
		, m_buffer(this,
				   &std::clog,
				   bus_ptr(new default_log_event_bus<stream_t, level_t>(this)),
				   string_type(),
				   LL_NORMAL,
				   LF_TIMESTAMP | LF_TRUNCATE)
		, m_threshold(LL_HIGHEST)
		{
		}

	virtual ~basic_logger() throw()
		{
			flush();
		}

	self_type		&operator<<(const level_t lvl)
		{
			m_buffer.level(lvl);
			return (*this);
		}

	level_type			level() const throw()
		{ return (m_buffer.level()); }
	basic_logger		&level(const level_type lvl) throw()
		{ m_buffer.level(lvl); return (*this); }

	level_type			threshold() const throw()
		{ return (m_threshold); }
	basic_logger		&threshold(const level_type lvl) throw()
		{ m_buffer.threshold(lvl); m_threshold = lvl; return (*this); }

	flag_type			flags() const throw()
		{ return (m_buffer.flags()); }
	basic_logger		&flags(const flag_type lvl) throw()
		{ m_buffer.flags(lvl); return (*this); }

	bool				has_flags(const flag_type f) const throw()
		{ return ((m_buffer.flags() & f) != 0); }

	bus_ptr				bus() const throw() { return (m_buffer.bus()); }
	void				bus(bus_ptr b) throw() { return (m_buffer.bus(b)); }
	self_type			&flush()
		{
			stream_type::flush();
			m_stream->flush();
			m_buffer.sync();
			return (*this);
		}
	self_type			&insert(const string_type &str,
								const level_type lvl = default_level)
		{
			m_buffer.level(lvl);
			stream_type::write(&str.at(0), str.size());
#ifdef UNICODE
			stream_type::operator<<(std::wendl);
#else
			stream_type::operator<<(std::endl);
#endif
			return (*this);
		}

	stream_type			&stream() throw() { return (*m_stream); }
	buf_type			&buffer() throw() { return (m_buffer); }

protected:
	stream_type			*m_stream;
	buf_type			m_buffer;
	level_type			m_threshold;
protected:

};


#endif
