/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AccessControl.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/10 23:29:51 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/29 20:08:12 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		ACCESSCONTROL_H
# define	ACCESSCONTROL_H

# include "stringlist.hpp"
# include "stringmap.hpp"
# include <sstream>

enum					AccessControlMode
{
	ACM_NONE,
	ACM_INCLUDE,
	ACM_EXCLUDE,
	ACM_COUNT
};

class					AccessControlSection
{
public:
	typedef AccessControlMode	Mode;

	AccessControlSection(const std::string &name, Mode mode = ACM_INCLUDE);
	AccessControlSection(const AccessControlSection &rk);
	~AccessControlSection();

	AccessControlSection		&operator=(const AccessControlSection &rk);
	AccessControlSection		&operator+=(const AccessControlSection &rk);

	AccessControlSection		&addHeader(const std::string &key,
										   const std::string &val);
	AccessControlSection		&addIP(const std::string &str);
	AccessControlSection		&addURI(const std::string &str);

	const StringMap				&getHeaders() const;
	const StringList			&getIPs() const;
	const StringList			&getURIs() const;

	AccessControlSection		&addAlias(const std::string &key,
										  const std::string &val);
	StringMap					getAliases() const;
	void						setAliases(const StringMap &m);

	Mode						getMode() const;
	void						setMode(Mode m);

	StringMap					&getHeaders();
	StringList					&getIPs();
	StringList					&getURIs();

	const std::string			&getName() const;
protected:
	std::string					mName;
	StringMap					mHeaders;
	StringList					mIPs;
	StringList					mURIs;
	StringMap					mAliases;
	Mode						mMode;

};
typedef std::shared_ptr<AccessControlSection> SharedAccessControlSection;
class					AccessControlList
{
public:
	typedef AccessControlMode							Mode;
	typedef AccessControlSection						Section;
	typedef SharedAccessControlSection					SharedSection;
	typedef std::map<std::string, SharedSection>		SectionList;
	typedef std::shared_ptr<SectionList>				SharedSectionList;

public:
	AccessControlList();
	AccessControlList(const AccessControlList &);
	~AccessControlList();

	void				parse(std::istream &is);
	void				serialize(std::ostream &os) const;

	AccessControlList	&operator=(const AccessControlList &);


	SharedSection		getSection(const std::string &name);
	const SharedSection	getSection(const std::string &name) const;

	SharedSection		getCurrentSection();
	const SharedSection	getCurrentSection() const;

	const SharedSectionList	getSections() const;

	void				merge(const AccessControlList &rk);

	SharedSection		addSection(const std::string &name,
								   Mode mode = ACM_INCLUDE);
	void				clearSections();

protected:
	SharedSectionList	mSections;
	SharedSection		mSection;

	friend std::istream &operator >>(std::istream &is, AccessControlList &acl);
	friend std::ostream &operator <<(std::ostream &os, const AccessControlList &acl);
};

std::istream &operator >>(std::istream &is, AccessControlList &acl);
std::ostream &operator <<(std::ostream &os, const AccessControlList &acl);

typedef std::shared_ptr<AccessControlList>	SharedAccessControlList;
#endif
