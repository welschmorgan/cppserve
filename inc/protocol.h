/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protocol.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 16:48:49 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/12 17:42:54 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		PROTOCOL_H
# define	PROTOCOL_H

# include "stringlist.h"

class						HTTPProtocol {
protected:
	std::string				mVersion;
	std::string				mURI;
	std::string				mMethod;

public:
	HTTPProtocol();
	HTTPProtocol(const HTTPProtocol &p);
	~HTTPProtocol();

	void					setVersion(const std::string &s);
	void					setURI(const std::string &s);
	void					setMethod(const std::string &s);

	const std::string		&getVersion() const;
	const std::string		&getURI() const;
	const std::string		&getMethod() const;

	std::string				stringify() const;
	std::ostream			&stringify(std::ostream &os) const;
	bool					parse(std::istream &is);

	static StringList		getMethods();
	static bool				isMethodName(const std::string &name);


	HTTPProtocol			&operator=(const HTTPProtocol &p);

	friend std::istream		&operator>>(std::istream &is, HTTPProtocol &rk);
	friend std::ostream		&operator<<(std::ostream &os, const HTTPProtocol &rk);
};

extern std::istream			&operator>>(std::istream &is, HTTPProtocol &rk);
extern std::ostream			&operator<<(std::ostream &os, const HTTPProtocol &rk);

#endif
