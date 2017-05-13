// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 14:54:50 by mwelsch           #+#    #+#             //
//   Updated: 2017/05/13 15:18:44 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <libio.hpp>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "unit_test.hpp"

using namespace io;

void				test_import_export()
{
	basic_config_node<char>			wroot("root", "my supa com");
	basic_config_node<char>			rroot;
	basic_config_serializer<char>	ser;
	std::stringstream				wbuf;
	std::stringstream				rbuf;

	wroot.create_child("test.a", "b")->comment("biatch");
	ser.stringify(wbuf, wroot);
	ser.parse(wbuf, rroot);
	ser.stringify(rbuf, rroot);
	if (rbuf.str() != wbuf.str())
		throw std::runtime_error("exported:\n" + wbuf.str() + "\n---------\nimported:\n" + rbuf.str());
	std::cout << wbuf.str() << std::endl;
}

int					main()
{
	unit_test::func<void> test1("import/export", "exports then re-imports a simple node", test_import_export);
	if (test1())
		return (1);
	return (0);
}
