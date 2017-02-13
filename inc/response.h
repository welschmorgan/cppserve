/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/11 11:35:11 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/11 13:49:46 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		RESPONSE_H
# define	RESPONSE_H

# include "stringlist.h"
# include "socket.h"
# include "status.h"

class HTTPResponse {
protected:
	std::string			mProto;

	bool				mAutoClose;
	bool				mWritten;

	int					mStatus;
	StringMap			mHeaders;
	std::string			mBody;

public:
	HTTPResponse(const std::string &proto = std::string(),
				 bool autoClose = true);
	HTTPResponse(const HTTPResponse &);
	~HTTPResponse();

	HTTPResponse		&operator=(const HTTPResponse &);

	void				setStatus(int code);
	void				setHeader(const std::string &name,
								  const std::string &value);
	void				setProto(const std::string &proto);
	void				setBody(const std::string &body);

	int					getStatus(int code) const;
	const StringMap		&getHeaders() const;
	const std::string	&getProto() const;
	const std::string	&getBody() const;

	void				reset(const std::string &proto = std::string(),
							  int status = HTTPStatus::Success::Ok);
	int					write(SocketStream &s);
};

	typedef std::shared_ptr<HTTPResponse> SharedHTTPResponse;

#endif
