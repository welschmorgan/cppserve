// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 14:54:50 by mwelsch           #+#    #+#             //
//   Updated: 2017/05/13 15:21:49 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <libio.hpp>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "unit_test.hpp"

using namespace io;

void				test_structure()
{
		typedef basic_config_node<char>	node_type;
		basic_config_serializer<char>	ser;
		std::stringstream				ss;
		node_type						root("root",
											 "my supa com");
		node_type						*a, *b, *c;
		root.create_child("a.b.c", "abc");
		ser.stringify(ss, root);
		a = root.child("a");
		b = a ? a->child("b") : NULL;
		c = b ? b->child("c") : NULL;
		if (!c)
		{
			throw std::runtime_error("structure doesn't match:\nwanted:\na\n\tb\n\t\tc\ngot:\n" + ss.str());
		}
		std::cerr << ss.str() << std::endl;
}

int					main()
{
	unit_test::func<void> test1("structure", "checks the structure of a made up node", test_structure);
	if (test1())
		return (1);
	return (0);
}
