/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/17 17:02:04 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/22 20:40:43 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		LOGGER_BASE_H
# define	LOGGER_BASE_H

# include "log_flags.hpp"
# include "log_level.hpp"
# include "basic_logger.hpp"
# include "basic_file_logger.hpp"

typedef basic_logger<OStream, log_level>		logger;
typedef basic_file_logger<OStream, log_level>	file_logger;

typedef std::shared_ptr<logger> shared_logger_ptr;
typedef std::shared_ptr<file_logger> shared_file_logger_ptr;

#endif
