// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.hpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 09:06:35 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/30 09:16:44 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#ifndef MAIN_HPP
# define MAIN_HPP

#include <sys/types.h>
#include <dirent.h>
#include <cstring>
#include <vector>


#ifdef UNICODE
typedef std::wstring			string;
#else
typedef std::string				string;
#endif
typedef std::vector<string>		string_list;

struct test_suite
{
	string_list					list;

	string_list					&discover(const string &path);
};

#endif
