// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   serializer.hpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/05/12 20:07:59 by mwelsch           #+#    #+#             //
//   Updated: 2017/05/13 15:31:46 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef LIBIO_SERIALIZER_HPP
# define LIBIO_SERIALIZER_HPP

# include "serializer_traits.hpp"

namespace io
{
	template<typename item_t,
			 typename char_t>
	class															basic_serializer
	{
	public:
		typedef basic_serializer_traits<item_t, char_t>				traits_type;
		typedef basic_serializer<item_t, char_t>					self_type;

		typedef typename traits_type::item_type						item_type;
		typedef typename traits_type::char_type						char_type;
		typedef typename traits_type::string_type					string_type;
		typedef typename traits_type::istream_type					istream_type;
		typedef typename traits_type::ostream_type					ostream_type;

	private:

	protected:
		friend ostream_type				&operator<<(ostream_type &, const item_t &);
		friend istream_type				&operator>>(istream_type &, item_t &);

		string_type						m_name;
		string_type						m_version;

	public:
		basic_serializer(const string_type &name, const string_type version = string_type("1.0.0"))
			: m_name(name)
			, m_version(version)
			{}
		basic_serializer(const self_type &rhs)
			: m_name(rhs.m_name)
			, m_version(rhs.m_version)
			{}
		virtual ~basic_serializer() throw() {}

		self_type						&operator=(const self_type &rhs)
			{
				m_name = rhs.m_name;
				m_version = rhs.m_version;
				return (*this);
			}

		const string_type				&name() const throw()
			{ return (m_name); }

		void							name(const string_type &name) throw()
			{ m_name = name; }


		const string_type				&version() const throw()
			{ return (m_version); }

		void							version(const string_type &version) throw()
			{ m_version = version; }

		virtual ostream_type			&stringify(ostream_type &os, const item_type &item) = 0;
		virtual istream_type			&parse(istream_type &is, item_type &item) = 0;
	};
}

#endif
