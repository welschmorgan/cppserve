// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 14:54:50 by mwelsch           #+#    #+#             //
//   Updated: 2017/05/01 11:49:15 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <libfs.hpp>
#include <vector>

static fs::path		write_temp_file(const fs::string &content,
									size_t &sz)
{
	std::fstream	ofs;
	fs::path::char_type	*cpath;
	fs::path		p("temp.txt");
	if ((cpath = getenv("TMPDIR")) != NULL
		|| (cpath = getenv("TEMP")) != NULL)
		p = fs::path(fs::string(cpath) + fs::path::sep + "test.tmp");
	ofs.open(p.c_str(), std::ios::in | std::ios::out | std::ios::trunc);
	if (!ofs)
		throw fs::error("failed to write file", p.str());
	ofs << content;
	ofs.flush();
	ofs.seekp(0, std::ios::end);
	sz = ofs.tellp();
	ofs.close();
	return (p);
}

int					test_valid_file()
{
	size_t			size;
	fs::string		content("hello im a derp");
	fs::path		path;

	path = write_temp_file(content, size);
	if (size != path.size())
	{
		std::cerr << "[\033[0;31m" << "KO" << "\033[0m]"
				  << path.str() << ": invalid file size: " << std::to_string(size) << " != " << path.size() << std::endl;
		return (1);
	}
	else
	{
		std::cout << "[\033[0;32m" << "OK" << "\033[0m]"
				  << path.str() << ": "
				  << path.size() << " bytes" << std::endl;
	}
	return (0);
}
int					test_invalid_file()
{
	fs::path		path("");

	if (path.size() != 0)
	{
		std::cerr << "[\033[0;31m" << "KO" << "\033[0m]"
				  << "invalid file size must be zero!"
				  << std::endl;
		return (1);
	}
	std::cerr << "[\033[0;32m" << "OK" << "\033[0m]"
			  << "invalid file size is zero!"
			  << std::endl;
	return (0);
}

int											main()
{
	if (test_valid_file())
		return (1);
	if (test_invalid_file())
		return (1);
	return (0);
}
