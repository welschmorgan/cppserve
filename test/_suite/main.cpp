// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 08:46:35 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/30 12:40:12 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#include "main.hpp"
#include <libprocess/process.hpp>

using namespace unit_test;
using namespace proc;

int					main()
{
	suite							suite;
	process							proc("/bin/ls");

	suite.discover("./");
	suite.run();
	proc.arg("-alh");
	proc.execute();
	return (0);
}
