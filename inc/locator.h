/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Locator.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/10 21:25:56 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/09 20:51:14 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef					LOCATOR_H
# define				LOCATOR_H

# include "stringlist.h"
# include "path.h"
# include <functional>
# include <list>

class											ResourceScanHandler;
typedef std::shared_ptr<ResourceScanHandler>	SharedResourceScanHandler;
typedef std::list<SharedResourceScanHandler>	ResourceScanHandlerList;

class											Locator
{
public:
	typedef std::function<void (Locator *,
								SharedStringList,
								StringList::iterator,
								void *)>		Handler;
	typedef std::map<std::string, Handler>		HandlerMap;

public:
	Locator(const std::string &base_dir = std::string());
	Locator(const Locator &);
	~Locator();

	Locator										&operator=(const Locator &);

	std::string									find(const std::string &name) const;
	void										discover(void *extra = NULL);

	void										setBaseDir(const std::string &dir);
	const std::string							&getBaseDir() const;

	SharedStringList							getFolders() const;
	void										clearFolders();

	void										onFileAdded(StringList::iterator it);
	void										onFolderAdded(StringList::iterator it);

	SharedStringList							getFiles() const;
	void										clearFiles();

	SharedStringList							getErrors() const;

	SharedResourceScanHandler					addHandler(SharedResourceScanHandler h);
	void										setHandlers(const ResourceScanHandlerList &l);
	ResourceScanHandlerList						&getHandlers();
	const ResourceScanHandlerList				&getHandlers() const;

	void										*getExtra();
	const void									*getExtra() const;
	void										setExtra(void *e);


protected:
	void										discoverFolder(const std::string &folder);

protected:
	std::string									mBaseDir;
	SharedStringList							mFolders;
	SharedStringList							mFiles;
	SharedStringList							mErrors;
	ResourceScanHandlerList						mHandlers;
	void										*mExtra;
};

class							ResourceScanHandler {
public:
	typedef Locator::Handler	Handler;

	ResourceScanHandler(Locator *loc,
						const std::string &name,
						const StringList &interests = StringList(),
						const Handler &h = Handler());
	ResourceScanHandler(const ResourceScanHandler &);
	virtual ~ResourceScanHandler();

	ResourceScanHandler			&operator=(const ResourceScanHandler &rk);

	Locator						*getLocator() throw();
	const Locator				*getLocator() const throw();

	void						operator()(Locator *,
										   SharedStringList ,
										   StringList::iterator,
										   void *);


	bool						shouldHandle(const Path &filepath) const;
	void						handle(Locator *locator,
									   SharedStringList strings,
									   StringList::iterator iter,
									   void *extra);

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
	Locator						*mLocator;
};

class							StaticResourceHandler
	: public ResourceScanHandler
{
public:
	StaticResourceHandler(Locator *loc);
	StaticResourceHandler(const StaticResourceHandler &);
	~StaticResourceHandler();

	StaticResourceHandler		&operator=(const StaticResourceHandler &rk);

protected:
	void						work(Locator *locator,
									 SharedStringList strings,
									 StringList::iterator iter,
									 void *extra);
};

typedef std::vector<std::string>				URITranslation;
typedef std::map<std::string, URITranslation>	URITranslationMap;
typedef std::map<std::string, std::string>		URITranslationResultMap;

class							URITranslationHandler
	: public ResourceScanHandler
{
public:
	URITranslationHandler(Locator *loc,
						  const URITranslationMap *tr,
						  URITranslationResultMap &res);
	URITranslationHandler(const URITranslationHandler &);
	~URITranslationHandler();

	URITranslationHandler		&operator=(const URITranslationHandler &rk);

protected:
	void						work(Locator *locator,
									 SharedStringList strings,
									 StringList::iterator iter,
									 void *extra);

	const URITranslationMap	*mTranslations;
	URITranslationResultMap	&mResults;
};

typedef std::string								ETag;
typedef std::hash<std::string>					ETagHash;
typedef std::map<std::string, ETag>				ETagMap;

class											ETagHandler
	: public ResourceScanHandler
{
public:
	ETagHandler(Locator *loc,
				ETagMap &tags);
	ETagHandler(const ETagHandler &);
	~ETagHandler();

	ETagHandler									&operator=(const ETagHandler &rk);

protected:
	void										work(Locator *locator,
													 SharedStringList strings,
													 StringList::iterator iter,
													 void *extra);

	ETagMap										&mTags;
};

class							ACLResourceHandler
	: public ResourceScanHandler
{
public:
	ACLResourceHandler(Locator *loc);
	ACLResourceHandler(const ACLResourceHandler &);
	~ACLResourceHandler();

	ACLResourceHandler		&operator=(const ACLResourceHandler &rk);

protected:
	void					work(Locator *locator,
								 SharedStringList strings,
								 StringList::iterator iter,
								 void *extra);
};

#endif
