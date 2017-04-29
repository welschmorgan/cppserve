// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/22 16:29:30 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/29 18:50:44 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

# include "main.hpp"

typedef launcher<String, OStream>		launcher_t;

int					main()
{
	SocketStream				sock(0, 0);
	launcher_t					launchr(std::clog, "localhost", 8080, 5000);
	try {
		launchr.start<void(launcher_t *, SocketStream &), launcher_t *, SocketStream &>(
			launcher_t::ping,
			&launchr,
			sock);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	launchr.print_report(std::cout);
	return (EXIT_SUCCESS);
}
