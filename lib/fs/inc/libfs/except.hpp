// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   except.hpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/30 13:28:38 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/30 13:37:50 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //
#ifndef LIBFS_EXCEPT_HPP
# define LIBFS_EXCEPT_HPP

# include <string>
# include <stdexcept>

namespace fs
{
	template<typename string_t>
	class										basic_error
		: public std::runtime_error
	{
	public:
		typedef std::runtime_error				base_type;
		typedef basic_error<string_t>			self_type;
		typedef typename string_t::value_type	char_type;
		typedef string_t						str_type;

		static str_type							make_message(const str_type &reason,
															 const str_type &entry = str_type()) throw()
			{
				str_type						msg;
				if (!entry.empty())
					msg = entry + ": " + reason;
				else
					msg = reason;
				return (reason);
			}

	protected:
		str_type								m_reason;
		str_type								m_entry;

	public:
		basic_error(const str_type &reason,
					const str_type &entry = string_t())
			: base_type(make_message(reason, entry))
			, m_reason(reason)
			, m_entry(entry)
			{}
		basic_error(const self_type &rhs)
			: base_type(rhs)
			, m_reason(rhs.m_reason)
			, m_entry(rhs.m_entry)
			{}
		virtual ~basic_error() throw()
			{}

		basic_error								&operator=(const self_type &rhs)
			{
				base_type::operator=(rhs);
				m_reason = rhs.m_reason;
				m_entry = rhs.m_entry;
				return (*this);
			}

		const str_type							&reason() const throw()
			{ return (m_reason); }

		const str_type							&entry() const throw()
			{ return (m_entry); }
	};
};

#endif
