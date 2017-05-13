#ifndef LIBIO_SERIALIZER_TRAITS_HPP
# define LIBIO_SERIALIZER_TRAITS_HPP

# include <list>
# include <string>
# include <ostream>
# include <istream>

namespace io
{
	template<typename item_t,
			 typename char_t>
	struct															basic_serializer_traits
	{
		typedef item_t												item_type;
		typedef char_t												char_type;
		typedef std::basic_string<char_t>							string_type;
		typedef std::list<string_type>							string_list_type;
		typedef std::basic_istream<char_t>							istream_type;
		typedef std::basic_ostream<char_t>							ostream_type;
	};
}

#endif
