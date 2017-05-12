// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 14:54:50 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/30 16:37:07 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <libfs.hpp>
#include <vector>

using namespace fs;

static fs::path		write_temp_file(const fs::string &content,
									size_t &hash)
{
	std::ofstream	ofs;
	fs::path::char_type	*cpath;
	fs::path		p("temp.txt");
	if ((cpath = getenv("TMPDIR")) != NULL
		|| (cpath = getenv("TEMP")) != NULL)
		p = fs::path(fs::string(cpath) + fs::path::sep + "test.tmp");
	ofs.open(p.c_str(), std::ios::out | std::ios::trunc);
	if (!ofs)
		throw fs::error("failed to write file", p.str());
	ofs << content;
	ofs.close();
	std::hash<fs::string> hasher;
	hash = hasher(content);
	return (p);

}

int					test_valid_hash()
{
	size_t			valid_hash;
	fs::path		valid_path;

	valid_path = write_temp_file("hello im a derp", valid_hash);
	if (valid_path.hash() != valid_hash)
	{
		std::cerr << "[\033[0;31m" << "KO" << "\033[0m]"
				  << valid_path.str() << ": invalid file hash: " << std::to_string(valid_hash) << " != " << valid_path.hash_str() << std::endl;
		return (1);
	}
	else
	{
		std::cout << "[\033[0;32m" << "OK" << "\033[0m]"
				  << valid_path.str() << ": "
				  << valid_path.hash_str() << std::endl;
	}
	return (0);
}
int					test_invalid_hash()
{
	fs::path		path;

	try {
		path = fs::path("");
		path.hash();
		std::cerr << "[\033[0;31m" << "KO" << "\033[0m]"
				  << "no exception was thrown for non-existing file!"
				  << std::endl;
		return (1);
	} catch (std::exception &ex) {
		std::cout << "[\033[0;32m" << "OK" << "\033[0m]"
				  << "invalid file: expected error: " << ex.what() << std::endl;
	}
	return (0);
}
int					main()
{
	if (test_valid_hash())
		return (1);
	if (test_invalid_hash())
		return (1);
	return (0);
}
