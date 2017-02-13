/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cache.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 18:22:01 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/12 18:57:56 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		CACHE_H
# define	CACHE_H

# include <string>
# include <memory>
# include "serializer.h"

class									CacheData
	: public Serializer<CacheData>
{
public:
	CacheData();
	CacheData(const std::string			&name,
			  const std::string			&mimes,
			  const size_t				size,
			  const unsigned char		*data);
	CacheData(const CacheData &rk);
	virtual ~CacheData();

	CacheData							&operator=(const CacheData &rk);

	std::string							getName() const;
	std::string							getMimeType() const;
	size_t								getSize() const;
	unsigned char						*getData() const;

	std::string							&setName(const std::string &rk);
	std::string							&setMimeType(const std::string &rk);
	size_t								&setSize(const size_t rk);
	unsigned char						*setData(const unsigned char *rk = NULL,
												 const size_t size = 0);
protected:
	std::string							mName;
	std::string							mMime;
	size_t								mSize;
	unsigned char						*mData;

	friend std::ostream					&operator<<(std::ostream &os,
													CacheData &c);
	friend std::istream					&operator<<(std::istream &is,
													const CacheData &c);
};

extern std::ostream						&operator<<(std::ostream &os,
													CacheData &c);
extern std::istream						&operator<<(std::istream &is,
													const CacheData &c);

typedef std::shared_ptr<CacheData>		SharedCacheData;

#endif
