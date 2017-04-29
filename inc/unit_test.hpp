/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_test.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/21 22:03:41 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/25 20:55:59 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		UNIT_TEST_H
# define	UNIT_TEST_H

# include <string>
# include <functional>
# include <iostream>
# include <sstream>
# include <chrono>

# ifdef UNICODE
typedef std::wstring			String;
typedef std::wstringstream		StringStream;
# else
typedef std::string				String;
typedef std::stringstream		StringStream;
# endif

namespace				unit_test
{
	template<typename ret_t, typename... args_t>
	class				func
	{
	public:
		typedef func<ret_t, args_t...>				self_type;
		typedef std::function<ret_t(args_t...)>		func_type;
		typedef ret_t								return_type;
		typedef std::chrono::high_resolution_clock	clock_type;
		typedef std::chrono::time_point<clock_type>	time_point_type;
		typedef String								string_type;
		typedef StringStream						string_stream_type;

		string_type								name;
		func_type								handler;
		time_point_type							start_time;
		time_point_type							end_time;
		string_stream_type						error;

		func(const string_type &name_ = string_type())
			: name(name_)
			, handler()
			, start_time(clock_type::now())
			, end_time(clock_type::now())
			, error()
			{}
		func(const self_type &rhs)
			: name(rhs.name)
			, handler(rhs.handler)
			, start_time(rhs.start_time)
			, end_time(rhs.end_time)
			, error(rhs.error)
			{}
		func(const string_type &name_,
			 func_type fn)
			: name(name_)
			, handler(fn)
			, start_time(clock_type::now())
			, end_time(clock_type::now())
			, error()
			{}
		virtual ~func() throw() {}

		self_type						&operator=(const self_type &rhs)
			{
				name = rhs.name;
				handler = rhs.handler;
				start_time = rhs.start_time;
				end_time = rhs.end_time;
				error = rhs.error;
				return (*this);
			}

		ret_t							operator()(args_t... args)
			{ return (run(args...)); }


		ret_t							run(args_t... args) throw (std::runtime_error)
			{
				ret_t					ret;

				start_time = clock_type::now();
				try {
					ret = handler(args...);
					end_time = clock_type::now();
				} catch (std::exception &ex) {
					if (!error.str().empty())
						error << std::endl;
					error << string_type(ex.what());
					std::cerr << string_type(ex.what()) << std::endl;
					end_time = clock_type::now();
				}
				return (ret);
			}

		ret_t							must_return(ret_t val,
													args_t... args) throw (std::runtime_error)
			{
				ret_t					ret;
				ret = run(args...);
				if (ret != val)
					throw std::runtime_error(name + " failed:\n\tshould have returned: " + std::to_string(val) + "\n\tbut returned instead: " + std::to_string(ret) + "!");
				return (ret);
			}
	};
};

#endif
