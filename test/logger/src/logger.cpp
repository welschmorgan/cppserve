// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   logger.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/15 14:17:16 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/25 22:15:39 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
# include <logger.hpp>
# include <unit_test.hpp>
# include <vector>

using namespace unit_test;

template<typename logger_t>
struct										logger_test
{
	typedef logger_test<logger_t>			self;
	typedef logger_t						logger;
	typedef typename logger::level_type		level;
	typedef typename logger::string_type	string;
	typedef std::map<level, std::string>	line_map;
	typedef std::pair<string,
					  string>				line_pair;
	typedef std::vector<line_pair>			line_pair_list;;
	typedef std::unique_ptr<logger>			logger_ptr;
	typedef std::vector<string>				string_list;

	static const string						filename;

	line_map								lines;
	logger_ptr								log;
	line_pair_list							result;

	logger_test()
		: lines()
		, log(new file_logger(self::filename,
							  LL_NORMAL,
							  LF_LEVEL | LF_TIMESTAMP))
		, result()
		{
			lines =
				{
					{ LL_NORMAL, "should print" },
					{ LL_LOW, "should also print" },
					{ LL_HIGH, "should not print" },
					{ LL_NORMAL, "should print" }
				};
		}

	void									run() throw (std::runtime_error) {
		typename string_list::iterator		itdiff;
		string								wname("---");
		string								rname("+++");
		string_list							diff;

		fill_log();
		fetch_log();
		if ((diff = diff_lines()).size())
		{
			std::cerr << wname << ": line written to logger" << std::endl;
			std::cerr << rname << ": line read back from logger" << std::endl;
			std::cerr << string(40, '=') << std::endl;
			for (itdiff = diff.begin(); itdiff != diff.end(); itdiff++)
				std::cerr << *itdiff << std::endl;
			throw std::runtime_error("lines written and read back from file are different!");
		}
	}

private:
	void									fill_log() throw (std::runtime_error) {
		typename line_map::iterator			it;
		for (it = lines.begin(); it != lines.end(); it++)
		{
			this->log->insert(it->second, it->first);
			if (it->first <= this->log->threshold())
				this->result.push_back(std::make_pair(it->second, string("")));
		}
		this->log->close();
	}
	void										fetch_log() throw (std::runtime_error) {
		std::ifstream							ifs(self::filename, std::ios::in);
		size_t									i = 0;
		std::string								line;
		size_t									pos;
		if (!ifs.is_open())
			throw std::runtime_error(self::filename + ": failed to read file!");
		while (ifs)
		{
			if (!std::getline(ifs, line))
				break ;
			if ((pos = line.find("|")) != std::string::npos)
				line = line.substr(pos + 1, line.size() - (pos + 1));
			while (!line.empty() && line[0] == ' ')
				line.erase(line.begin());
			if (i >= this->result.size())
				this->result.push_back(std::make_pair(string(""), line));
			else
				this->result[i].second = line;
			i++;
		}
	}
	std::vector<string>							diff_lines() throw (std::runtime_error) {
		typename line_pair_list::const_iterator	itline;
		std::vector<string>						diff;
		string									wname("---");
		string									rname("+++");

		for (itline = result.begin(); itline != result.end(); itline++)
		{
			if (itline->first != itline->second)
			{
				diff.push_back(wname + ": " + itline->first);
				diff.push_back(rname + ": " + itline->second);
			}
		}
		return (diff);
	}
};

template<>
const logger_test<file_logger>::string					logger_test<file_logger>::filename("test.log");

int main()
{
	logger_test<file_logger>	t;
	try {
		t.run();
	} catch (std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
