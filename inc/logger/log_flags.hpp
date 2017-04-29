/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_flags.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 17:07:13 by mwelsch           #+#    #+#             */
/*   Updated: 2017/04/17 17:07:33 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		LOG_FLAGS_H
# define	LOG_FLAGS_H

# include <map>
# include "string.hpp"

enum									log_flag
{
	LF_NONE = 0,
	LF_TIMESTAMP = 1 << 1,				/*!< Shows timestamp of message */
	LF_TRUNCATE = 1 << 2,				/*!< Truncates log on construction */
	LF_LEVEL = 1 << 2,					/*!< Shows message level */
	LF_COUNT = 3
};

typedef std::map<log_flag, String>		log_flag_map;

const log_flag_map						*log_flags() throw();

OStream									&operator<<(OStream &os,
													log_flag f);

#endif
