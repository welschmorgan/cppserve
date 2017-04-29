/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_file_logger.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 17:05:36 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/23 14:43:02 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		BASIC_FILE_LOGGER_H
# define	BASIC_FILE_LOGGER_H

# include "basic_logger.hpp"
# include "file_event_bus.hpp"
# include "file_flags.hpp"
# include <fstream>

template<typename stream_t,
		 typename level_t>
class basic_file_logger
	: public basic_logger<stream_t,
						  level_t>
{
	basic_file_logger(const basic_file_logger &) = delete;
	basic_file_logger &operator=(const basic_file_logger &) = delete;

public:
	typedef basic_logger<stream_t, level_t>			base_type;
	typedef basic_file_logger<stream_t, level_t>	self_type;

	typedef typename base_type::stream_type			stream_type;
	typedef typename base_type::level_type			level_type;
	typedef typename base_type::flag_type			flag_type;
	typedef typename base_type::string_type			string_type;
	typedef typename base_type::buf_type			buf_type;
	typedef typename base_type::bus_ptr				bus_ptr;
	typedef typename base_type::bus_type			bus_type;
	typedef std::shared_ptr<OFStream>				fstream_ptr;

	static const flag_type						default_flags = FLF_NAME | LF_TRUNCATE | LF_TIMESTAMP | LF_LEVEL;
	static const level_type						default_level = LL_NORMAL;

protected:
	string_type		m_name;
	fstream_ptr		m_fstream;

public:
	basic_file_logger()
		: base_type(&std::clog,
					self_type::default_level,
					self_type::default_flags)
		, m_name()
		, m_fstream()
		{ init("default", false); }

	basic_file_logger(const string_type &name,
					  const level_type lvl = self_type::default_level,
					  const flag_type flags = self_type::default_flags)
		: base_type(&std::clog, lvl, flags)
		, m_name()
		, m_fstream()
		{ init(name, true); }

	basic_file_logger(fstream_ptr os,
					  level_type lvl = base_type::default_level,
					  flag_type flags = base_type::default_flags)
		: base_type(&std::clog, bus_ptr(), lvl, flags)
		, m_name()
		, m_fstream(os)
		{ init("fstream", false); }

	virtual ~basic_file_logger() throw()
		{ close(); }

	const string_type		&name() const throw() { return (m_name); }
	const OFStream			*fstream() const throw() { return (m_fstream.get()); }
	OFStream				*fstream() throw() { return (m_fstream.get()); }

	bool					open(const string_type &name,
								 std::ios_base::openmode stdflags = std::ios::out)
		{
			m_name = name;
			if ((stdflags & std::ios::out) == 0)
				stdflags |= std::ios::out;
			if ((base_type::flags() & LF_TRUNCATE) != LF_NONE)
				stdflags |= std::ios::trunc;
			m_fstream = fstream_ptr(new OFStream(m_name,
												 stdflags));
			return (m_fstream->is_open());
		}

	void					close()
		{
			base_type::flush();
			if (m_fstream)
			{
				m_fstream->flush();
				m_fstream->close();
				m_fstream.reset();
			}
		}

		bool				is_open() const throw()
			{
				return (m_fstream && m_fstream->is_open());
			}
private:
	void					init(const string_type &name = string_type(), bool openfile = false)
		{
			typedef file_log_event_bus<stream_t, level_t> file_bus_type;
			base_type		*base;

			base = (dynamic_cast<base_type*>(this));
			base_type::bus(bus_ptr(new file_bus_type(base)));
			if (!name.empty())
				m_name = name;
			if (openfile && !open(m_name))
				throw std::runtime_error(m_name + ": failed to open file: " + std::string(strerror(errno)));
		}

};

#endif
