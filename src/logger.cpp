// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   logger.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/16 19:36:46 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/22 15:33:09 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "logger.hpp"
#include <iostream>

const log_flag_map						*log_flags() throw() {
	static const log_flag_map			flags
	{
		{LF_NONE, "none"},
		{LF_TIMESTAMP, "timestamp"},
		{LF_TRUNCATE, "truncate"},
	};
	return (&flags);
}

const log_level_map						*log_levels() throw() {
	static const log_level_map			levels
	{
		{LL_LOWEST, "lowest"},
		{LL_LOW, "low"},
		{LL_NORMAL, "normal"},
		{LL_HIGH, "high"},
		{LL_HIGHEST, "highest"}
	};
	return (&levels);
}

OStream									&operator<<(OStream &os,
													log_flag f)
{
	const log_flag_map					*flags(log_flags());
	log_flag_map::const_iterator		it = flags->find(f);
	if (it == flags->end())
		it = flags->begin();
	os << it->second;
	return (os);
}

OStream									&operator<<(OStream &os,
													log_level f)
{
	const log_level_map					*levels(log_levels());
	log_level_map::const_iterator		it = levels->find(f);
	if (it == levels->end())
		it = levels->begin();
	os << it->second;
	return (os);
}
