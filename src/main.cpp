// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/04 13:28:39 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/14 19:50:17 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <unistd.h>
#include <iostream>
#include "server.hpp"
#include "path.hpp"

int	main(int argc, char *const argv[]) {
	int code = EXIT_SUCCESS;
	std::unique_ptr<HTTPServer> srv(new HTTPServer(argc, argv));
	try {
		code = srv->run();
	} catch (std::exception &e) {
		std::cerr << "[-] " << e.what() << std::endl;
	}
	return (code);
}
