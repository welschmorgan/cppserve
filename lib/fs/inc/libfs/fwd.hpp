// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   fwd.hpp                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 13:14:01 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/30 17:59:47 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#ifndef LIBFS_FWD_HPP
# define LIBFS_FWD_HPP

# include <string>

namespace		fs
{
	template<typename string_t>
	class								basic_error;
	template<typename string_t>
	class								basic_path;

	#ifdef UNICODE
	typedef std::wstring				string;
	typedef std::wostream				ostream;
	#else
	typedef std::string					string;
	typedef std::ostream				ostream;
	#endif

	template<typename char_t>
	class										basic_cstream_base;
	template<typename char_t>
	class										basic_cbuf;

	template<typename char_t>
	class										basic_icstream;

	template<typename char_t>
	class										basic_ocstream;

	typedef basic_error<string>			error;
	typedef basic_path<string>			path;
};

#endif
