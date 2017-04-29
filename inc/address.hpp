/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   address.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/10 20:33:55 by mwelsch           #+#    #+#             */
/*   Updated: 2017/02/10 20:58:54 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		ADDRESS_H
# define	ADDRESS_H

# include <string>
# include <arpa/inet.h>

class Address
{
public:
	Address(sockaddr_in *addr);
	Address(int family = int(0),
			const std::string &addr = std::string(),
			uint16_t port = uint16_t(0));
	Address(const Address &rk);
	~Address();

	Address &operator=(const Address &rk);

	void				update(int family,
							   const std::string &addr,
							   uint16_t port);

	int					getFamily() const;
	const std::string	&getAddress() const;
	uint16_t			getPort() const;

	operator			std::string() const;

	bool				operator!=(const Address &rk);
	bool				operator==(const Address &rk);

protected:
	int					mFamily;
	std::string			mAddr;
	uint16_t			mPort;
	friend std::ostream &operator<<(std::ostream &os, const Address &);
};

std::ostream &operator<<(std::ostream &os, const Address &);


#endif
