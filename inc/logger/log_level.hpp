/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_level.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 17:07:53 by mwelsch           #+#    #+#             */
/*   Updated: 2017/04/17 17:07:55 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		LOG_LEVEL_H
# define	LOG_LEVEL_H

# include "string.hpp"
# include <map>

enum									log_level
{
	LL_LOWEST,
	LL_LOW,
	LL_NORMAL,
	LL_HIGH,
	LL_HIGHEST,
	LL_COUNT
};
typedef std::map<log_level, String>		log_level_map;

const log_level_map						*log_levels() throw();

OStream									&operator<<(OStream &os,
													log_level f);

#endif
