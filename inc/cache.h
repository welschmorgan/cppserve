/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cache.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 18:22:01 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/09 20:10:40 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		CACHE_H
# define	CACHE_H

# include <string>
# include <memory>
# include <map>
# include "serializer.h"

class									CacheData
	: public Serializer<CacheData>
{
private:
	CacheData(const CacheData &rk);
	CacheData							&operator=(const CacheData &rk);

public:
	CacheData();
	CacheData(const std::string			&name,
			  const std::string			&mimes,
			  const std::stringbuf		*data);
	virtual ~CacheData();

	const std::string					&getName() const throw();
	const std::string					&getMimeType() const throw();
	size_t								getSize() const throw();
	const std::stringbuf				&getData() const throw();

	std::string							&setName(const std::string &rk);
	std::string							&setMimeType(const std::string &rk);
	std::stringbuf						&setData(const std::stringbuf &rk);

	time_t								getAccessTime() const throw();
	time_t								getCreationTime() const throw();
	time_t								getModificationTime() const throw();

	void								setAccessTime(time_t t) throw();
	void								setCreationTime(time_t t) throw();
	void								setModificationTime(time_t t) throw();

	bool								isLoaded() const throw();
	bool								load(const std::string &path);

protected:
	std::string							mName;
	std::string							mMime;
	std::stringbuf						mData;
	time_t								mCreateTime;
	time_t								mModTime;
	time_t								mAccTime;
	bool								mLoaded;

};

typedef std::shared_ptr<CacheData>				SharedCacheData;
typedef std::map<std::string, SharedCacheData>	CacheDataMap;

#endif
