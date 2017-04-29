// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   file_event_bus.hpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/23 14:39:05 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/23 14:43:38 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#ifndef FILE_EVENT_BUS__HPP
# define FILE_EVENT_BUS__HPP

# include "event_bus.hpp"
# include "file_flags.hpp"

template<typename stream_t,
		 typename level_t>
class											file_log_event_bus
	: public default_log_event_bus<stream_t, level_t>
{
	file_log_event_bus(const file_log_event_bus &) = delete;
	file_log_event_bus &operator=(const file_log_event_bus &) = delete;

protected:

public:
	typedef default_log_event_bus<stream_t, level_t>	base_type;
	typedef typename base_type::buf_type		buf_type;
	typedef typename base_type::char_type		char_type;
	typedef typename base_type::string_type		string_type;
	typedef typename base_type::level_type		level_type;
	typedef typename base_type::flag_type		flag_type;
	typedef typename base_type::logger_type		logger_type;

	file_log_event_bus(logger_type *parent = NULL)
		: base_type(parent)
		{}
	virtual ~file_log_event_bus() throw()
		{}

	void										format_entry(std::ostream *dup = NULL) {
		basic_file_logger<stream_t, level_t>	*parent(
			dynamic_cast<basic_file_logger<stream_t, level_t>*>(base_type::m_parent));
		OStream									*os(parent->fstream());
		if (parent->has_flags(FLF_NAME))
		{
			(*os) << "[" << parent->name() << "] ";
			if (dup)
				(*dup) << "[" << parent->name() << "] ";
		}
		base_type::format_entry(os);
		if (dup)
			base_type::format_entry(dup);
	}
};

#endif
