/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Locator.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/10 21:25:56 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/12 21:22:27 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef					LOCATOR_H
# define				LOCATOR_H

# include "stringlist.h"
# include "path.h"
# include <functional>
# include <list>

class										ResourceScanHandler;
typedef std::list<ResourceScanHandler>		ResourceScanHandlerList;

class					Locator
{
public:
	typedef std::function<void (Locator *, StringList::iterator, void *)>	Handler;
	typedef std::map<std::string, Handler>					HandlerMap;

public:
	Locator(const std::string &base_dir = std::string());
	Locator(const Locator &);
	~Locator();

	Locator								&operator=(const Locator &);

	std::string							find(const std::string &name) const;
	void								discover();

	void								setBaseDir(const std::string &dir);
	const std::string					&getBaseDir() const;

	SharedStringList					getFolders() const;
	void								clearFolders();

	void								onFileAdded(const std::string &path);
	void								onFolderAdded(const std::string &path);

	SharedStringList					getFiles() const;
	void								clearFiles();

	SharedStringList					getErrors() const;
	void								setHandlers(const ResourceScanHandlerList &l);
	ResourceScanHandlerList				&getHandlers();
	const ResourceScanHandlerList		&getHandlers() const;
protected:
	void				discoverFolder(const std::string &folder);

protected:
	std::string				mBaseDir;
	SharedStringList		mFolders;
	SharedStringList		mFiles;
	SharedStringList		mErrors;
	ResourceScanHandlerList mHandlers;
};

class							ResourceScanHandler {
public:
	typedef						std::function<void(Locator*,
												   StringList &strings,
												   StringList::iterator it,
												   void *extra)> Handler;

	ResourceScanHandler(const std::string &name,
						const StringList &interests = StringList(),
						const Handler &h = Handler());
	ResourceScanHandler(const ResourceScanHandler &);
	virtual ~ResourceScanHandler();

	ResourceScanHandler			&operator=(const ResourceScanHandler &rk);

	void						operator()(Locator *, StringList &, StringList::iterator, void *);


	bool						shouldHandle(const Path &filepath) const;
	void						handle(Locator *, StringList &, StringList::iterator, void *);

	const std::string			&getName() const;
	const StringList			&getInterests() const;
	const Handler				&getHandler() const;

	std::string					&setName(const std::string &rk);
	StringList					&setInterests(const StringList &rk);
	Handler						&setHandler(const Handler &rk);

protected:
	std::string					mName;
	StringList					mInterests;
	Handler						mHandler;
};

class							StaticResourceHandler
	: public ResourceScanHandler
{
public:
	StaticResourceHandler();
	StaticResourceHandler(const StaticResourceHandler &);
	~StaticResourceHandler();

	StaticResourceHandler		&operator=(const StaticResourceHandler &rk);
};

#endif
