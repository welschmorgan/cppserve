// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   host.hpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/22 17:38:56 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/29 16:12:03 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef HOST_HPP
# define HOST_HPP

# include "string.hpp"

class				Host
{
	typedef uint16_t	port_type;
public:
	Host();
	Host(const Host &);
	Host(const String &name,
		 const port_type port = port_type(80));
	~Host() throw();

	Host			&operator=(const Host &);

	Host			&name(const String &name) throw();
	Host			&port(const port_type nb) throw();

	String			name() const throw();
	port_type		port() const throw();

protected:
	String			mName;
	port_type		mPort;

friend OStream		&operator<<(OStream &os, const Host &host);
friend IStream		&operator>>(IStream &is, Host &host);

};

OStream				&operator<<(OStream &os, const Host &host);
IStream				&operator>>(IStream &is, Host &host);

#endif
