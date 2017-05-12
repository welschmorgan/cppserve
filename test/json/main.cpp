// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/29 21:33:07 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/29 22:31:03 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "json.hpp"

struct					test
{
	std::stringstream	input;
	std::stringstream	must_output;
};
int				main()
{
	typedef std::map<std::string, test> test_map;
	json::parser		parser;
	json::node			*root;
	json::text_serializer	ser;

	test_map tests;

	tests["array"].input.str("[ \"a value\", 42, ]");
	tests["array"].must_output << "\"root\": [" << std::endl
							   << "a value," << std::endl
							   << "42," << std::endl
							   << "]";
	std::stringstream	is;
	std::stringstream	res;
	uint16_t			lineid;
	uint16_t			diff_lines;

	for (test_map::iterator it = tests.begin(); it != tests.end(); it++)
	{
		is.clear();
		is.str(it->second.input.str());
		std::cout << "test: " << it->first << std::endl;
		root = parser.parse(is);
		res.clear();
		res.str("");
		ser.serialize(*root, res, 1);
		lineid = 0;
		diff_lines = 0;
		while (true)
		{
			std::string	linea, lineb;
			if (!std::getline(res, linea))
				break ;
			if (!std::getline(it->second.must_output, lineb))
				break ;
			json::parser::trim_quotes(linea);
			json::parser::trim_quotes(lineb);
			if (linea != lineb)
			{
				std::cerr << "[!" << lineid << "] " << linea << " != " << lineb << std::endl;
				diff_lines++;
			}
			lineid++;
		}
		if (diff_lines)
		{
			std::cerr << "expected: " << it->second.must_output.str() << std::endl;
			std::cerr << "got: " << res.str() << std::endl;
			std::cout << "\t\033[0;31mKO\033[0m" << std::endl;
		}
		else
			std::cout << "\t\033[0;32mOK\033[0m" << std::endl;
	}
	return (0);
}
