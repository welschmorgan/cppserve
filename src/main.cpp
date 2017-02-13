// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/02/04 13:28:39 by mwelsch           #+#    #+#             //
//   Updated: 2017/02/13 21:41:38 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <unistd.h>
#include <iostream>
#include "server.h"
#include "path.h"

int	main(int argc, char *const argv[]) {
	/*
	  PatternMatch<std::string>("h*.txt", "hello.txt");
	PatternMatch<std::string>("h*.txt", "hellotxt");
	PatternMatch<std::string>("*", "hellotxt");
	*/
	Path p("/hello/im/a/simple.path");
	std::cout << "dir: " << p.getDir() << std::endl;
	std::cout << "base: " << p.getBase(false) << std::endl;
	std::cout << "ext: " << p.getExt() << std::endl;
	return (0);
	int code = EXIT_SUCCESS;
	std::unique_ptr<HTTPServer> srv(new HTTPServer(argc, argv));
	try {
		code = srv->run();
	} catch (std::exception &e) {
		std::cerr << "[-] " << e.what() << std::endl;
	}
	return (code);
}
