// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.hpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 09:06:35 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/30 15:28:39 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#ifndef MAIN_HPP
# define MAIN_HPP

#include <string>
#include <stdexcept>
#include <vector>

namespace unit_test
{
#ifdef UNICODE
	typedef std::wstring			string;
#else
	typedef std::string				string;
#endif
	typedef std::vector<string>		string_list;


	class							error
		: public std::runtime_error
	{
	private:
		string						m_name;
		int							m_code;

	public:
		error(const string &msg_,
			  const string &name_ = string(),
			  const int code_ = 0);
		error(const error &);
		virtual ~error() throw();

		error						&operator=(const error &);

		const string				&name() const throw();
		int							code() const throw();
	};

	struct suite
	{
		string_list					list;
		int							pipefd[2];

		string_list					&discover(const string &path);
		int							run() throw(error);
	};
};

#endif
