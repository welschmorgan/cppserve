/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/15 14:14:16 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/23 13:06:34 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		STRING_H
# define	STRING_H

# include	<string>
# include	<fstream>
# include	<iostream>
# include	<sstream>

# ifdef UNICODE
typedef std::wstring			String;
typedef std::wstringstream		StringStream;
typedef std::wostream			OStream;
typedef std::wistream			IStream;
typedef std::wofstream			OFStream;
typedef std::wifstream			IFStream;
# else
typedef std::string				String;
typedef std::stringstream		StringStream;
typedef std::ostream			OStream;
typedef std::istream			IStream;
typedef std::ofstream			OFStream;
typedef std::ifstream			IFStream;

/*extern int strncmp(const char *s1, const char *s2, size_t n);
  extern int strcmp(const char *s1, const char *s2);*/

# endif


extern "C" {
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
}

# include <cstring>

#endif
