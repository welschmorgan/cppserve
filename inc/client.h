/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/05 13:56:14 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/05 18:40:56 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		CLIENT_H
# define	CLIENT_H

# include <string>
# include "socket.h"
# include <memory>

class HTTPClient {
protected:
	SocketStream::ptr	mStream;
	short				mFamily;
	std::string			mAddress;
	uint16_t			mPort;
	int					mPID;

private:
	HTTPClient(const HTTPClient &c);
	HTTPClient &operator=(const HTTPClient &rk);

public:
	HTTPClient(SocketStream::ptr strm,
			   short family,
			   const std::string &addr,
			   uint16_t port);
	virtual ~HTTPClient();

	operator std::string() const;

	int					getPID() const;
	void				_setPID(int pid);

	short				getFamily() const;
	std::string			getAddress() const;
	uint16_t			getPort() const;

	SocketStream::ptr	getStream() const;

	bool				open(const std::string &host, uint16_t port);
	void				close();

	friend std::ostream	&operator<<(std::ostream &os, const HTTPClient &);
};

typedef std::shared_ptr<HTTPClient> SharedHTTPClientPtr;

std::ostream			&operator<<(std::ostream &os, const HTTPClient &);

#endif
