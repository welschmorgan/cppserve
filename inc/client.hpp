/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/05 13:56:14 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/22 13:52:12 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		CLIENT_H
# define	CLIENT_H

# include <string>
# include <memory>
# include "address.hpp"
# include "socket.hpp"
# include "request.hpp"
# include "response.hpp"

class HTTPClient {
protected:
	SocketStream::ptr	mStream;
	Address				mAddress;
	int					mPID;
	SharedHTTPRequest	mRequest;

private:
	HTTPClient(const HTTPClient &c);
	HTTPClient &operator=(const HTTPClient &rk);

public:
	HTTPClient(SocketStream::ptr strm,
			   const Address &addr);
	virtual ~HTTPClient();

	SharedHTTPRequest					getRequest() const;
	operator std::string() const;
	void								parseRequest(const std::string &method,
													 const std::string &uri,
													 const std::string &proto);
	HTTPProtocol		getProtocol() const;

	int					getPID() const;
	void				_setPID(int pid);

	void				parseRequest();

	const Address		&getAddress() const;
	SocketStream::ptr	getStream() const;

	bool				writeResponse(SharedHTTPResponse resp);

	bool				open(const std::string &host, uint16_t port);
	void				close();

	friend std::ostream	&operator<<(std::ostream &os, const HTTPClient &);
};

typedef std::shared_ptr<HTTPClient> SharedHTTPClientPtr;

std::ostream			&operator<<(std::ostream &os, const HTTPClient &);

#endif
