// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   test_suite.cpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 09:05:58 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/30 15:29:51 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "main.hpp"
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

namespace unit_test
{
	string_list						&suite::discover(const string &path)
	{
		DIR				*dd;
		struct dirent	*de;
		dd = opendir(path.c_str());
		while ((de = readdir(dd)) != NULL)
		{
			if (strncmp(de->d_name, ".", 3)
				&& strncmp(de->d_name, "..", 3)
				&& ((de->d_type & DT_DIR) != 0)
				&& strncmp(de->d_name, "_suite", 7))
			{
				list.push_back(de->d_name);
			}
		}
		return (list);
	}

	int								suite::run() throw(error)
	{
		int					ret = 0;
		if (pipe(pipefd) == -1)
			throw error(strerror(errno), "", errno);
		for(auto test : list)
			std::cout << "found test: " << test << std::endl;
		return (ret);
	}


	error::error(const string &msg_,
				 const string &name_,
				 const int code_)
		: std::runtime_error(msg_)
		, m_name(name_)
		, m_code(code_)
	{}
	error::error(const error &rhs)
		: std::runtime_error(rhs)
	{}
	error::~error() throw()
	{}

	error						&error::operator=(const error &rhs)
	{
		std::runtime_error::operator=(rhs);
		m_name = rhs.m_name;
		m_code = rhs.m_code;
		return (*this);
	}

	const string				&error::name() const throw()
	{
		return (m_name);
	}
	int							error::code() const throw()
	{
		return (m_code);
	}

}
