// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/22 16:29:30 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/22 17:50:28 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

# include "request.hpp"

void				connect(SocketStream &sock) throw (std::runtime_error)
{
	String			host("localhost");
	uint16_t		port(8080);
	String			address;
	StringStream	ss;

	std::cout << "Address (host[=localhost]:port[=8000]): " << std::endl;
	std::getline(std::cin, address);
	if (!address.empty())
	{
		size_t	pos(address.find(':'));
		if (pos != std::string::npos)
		{
			host = address.substr(0, pos);
			std::cout << "host: " << host << std::endl;
			ss.str(address.substr(pos + 1));
			if (!(ss >> port))
				throw std::runtime_error("failed to parse address from '" + address + "'!");
			std::cout << "port: " << port << std::endl;
		}
	}
	else
		address = (host + ":" + std::to_string(port));
	if (!sock.open(host, port))
		throw std::runtime_error("failed to open socket to " + address);
	std::cout << "connected to " << address << "!" << std::endl;
}

int					main()
{
	SocketStream	sock(0, 0);

	try {
		connect(sock);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
