/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/11 02:31:05 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/22 16:50:06 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		REQUEST_H
# define	REQUEST_H

# include "stringlist.hpp"
# include "stringmap.hpp"
# include "protocol.hpp"
# include "socket.hpp"

class HTTPRequest
{
public:
	HTTPRequest();
	HTTPRequest(SharedStringMap		headers,
				SharedStringList	body,
				const HTTPProtocol	&proto,
				const std::string	&extra);
	HTTPRequest(const HTTPRequest &r);
	~HTTPRequest();

	HTTPRequest			&operator=(const HTTPRequest &rk);

	std::string			getHeader(const std::string name,
								  bool *found = NULL) const throw();

	bool				hasHeader(const std::string name) const throw();

	SharedStringMap		getHeaders() const;
	String				getHeader(const String &k) const;
	SharedStringList	getBody() const;
	std::string			getURI() const;
	std::string			getVersion() const;
	std::string			getMethod() const;
	std::string			getExtra() const;
	HTTPProtocol		getProtocol() const;

	HTTPProtocol		&setProtocol(const HTTPProtocol &p);
	HTTPProtocol		&setProtocol(const String &method,
									 const String &uri,
									 const String &version = "HTTP/1.1");
	SharedStringMap		setHeaders(const SharedStringMap &m);
	HTTPRequest			&setHeader(const String &k,
								   const String &v);
	SharedStringList	setBody(const SharedStringList &l);
	std::string			setURI(const std::string &uri);
	std::string			setVersion(const std::string &proto);
	std::string			setMethod(const std::string &meth);
	std::string			setExtra(const std::string &extra);

	bool				parse(SocketStream &is);
	size_t				write(SocketStream &os);

protected:
	SharedStringMap		mHeaders;
	SharedStringList	mBody;
	HTTPProtocol		mProto;
	std::string			mExtra;

};

typedef std::shared_ptr<HTTPRequest> SharedHTTPRequest;
#endif
