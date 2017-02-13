/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/11 02:31:05 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/12 17:34:49 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		REQUEST_H
# define	REQUEST_H

# include "stringlist.h"
# include "stringmap.h"
# include "protocol.h"
# include "socket.h"

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

	SharedStringMap		getHeaders() const;
	SharedStringList	getBody() const;
	std::string			getURI() const;
	std::string			getVersion() const;
	std::string			getMethod() const;
	std::string			getExtra() const;
	HTTPProtocol		getProtocol() const;

	HTTPProtocol		&setProtocol(const HTTPProtocol &p);
	SharedStringMap		setHeaders(const SharedStringMap &m);
	SharedStringList	setBody(const SharedStringList &l);
	std::string			setURI(const std::string &uri);
	std::string			setVersion(const std::string &proto);
	std::string			setMethod(const std::string &meth);
	std::string			setExtra(const std::string &extra);

	bool				parse(SocketStream &is);

protected:
	SharedStringMap		mHeaders;
	SharedStringList	mBody;
	HTTPProtocol		mProto;
	std::string			mExtra;

};

typedef std::shared_ptr<HTTPRequest> SharedHTTPRequest;
#endif
