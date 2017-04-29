/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forward.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 17:08:38 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/22 21:57:01 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		FORWARD_H
# define	FORWARD_H

# include "log_flags.hpp"
# include "log_level.hpp"
# include "string.hpp"
# include <memory>

template<typename stream_t, typename level_t = log_level>
class											log_event_bus;

template<typename stream_t, typename level_t = log_level>
using log_event_bus_ptr = std::shared_ptr<log_event_bus<stream_t, level_t>>;

template<typename stream_t, typename level_t = log_level>
class											default_log_event_bus;

template<typename stream_t, typename level_t = log_level>
class											file_log_event_bus;

template<typename stream_t, typename level_t = log_level>
class											basic_logger_buf;

template<typename stream_t, typename level_t = log_level>
class											basic_logger;

template<typename stream_t, typename level_t = log_level>
class											basic_file_logger;

#endif
