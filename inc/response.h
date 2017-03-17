/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/11 11:35:11 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/14 20:23:46 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		RESPONSE_H
# define	RESPONSE_H

# include "stringlist.h"
# include "socket.h"
# include "status.h"
# include "protocol.h"

class HTTPResponse {
protected:
	HTTPProtocol		mProto;

	bool				mAutoClose;
	bool				mWritten;

	int					mStatus;
	StringMap			mHeaders;
	std::string			mBody;

public:
	HTTPResponse(const HTTPProtocol &proto = HTTPProtocol(),
				 bool autoClose = true);
	HTTPResponse(const HTTPResponse &);
	~HTTPResponse();

	HTTPResponse		&operator=(const HTTPResponse &);

	void				setStatus(int code);
	void				setHeader(const std::string &name,
								  const std::string &value);
	void				setBody(const std::string &body);

	int					getStatus(int code) const;
	const StringMap		&getHeaders() const;
	const HTTPProtocol	&getProtocol() const;
	const std::string	&getBody() const;

	void				reset(const HTTPProtocol &proto = HTTPProtocol(),
							  int status = HTTPStatus::Success::Ok);
	int					write(SocketStream &s);
};

	typedef std::shared_ptr<HTTPResponse> SharedHTTPResponse;

#endif
