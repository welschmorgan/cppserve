// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   manual.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/23 15:39:04 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/23 17:13:51 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "manual.hpp"

ManualSection::ManualSection()
	: name()
	, title()
	, lines()
	, author()
	, desc()
	, active(false)
	, created_at(Clock::now())
{}
ManualSection::ManualSection(const String &name_,
					   const String &title_,
					   const StringStream &lines_,
					   const String &author_,
					   const StringStream &desc_)
	: name(name_)
	, title(title_)
	, lines()
	, author(author_)
	, desc()
	, active(false)
	, created_at(Clock::now())
{
	lines.str(lines_.str());
	desc.str(desc_.str());
}
ManualSection::ManualSection(const ManualSection &rhs)
	: name(rhs.name)
	, title(rhs.title)
	, lines()
	, author(rhs.author)
	, desc()
	, active(rhs.active)
	, created_at(rhs.created_at)
{
	lines.str(rhs.lines.str());
	desc.str(rhs.desc.str());
}

ManualSection::~ManualSection() throw()
{}

ManualSection				&ManualSection::operator=(const ManualSection &rhs) {
	name = rhs.name;
	title = rhs.title;
	lines.str(rhs.lines.str());
	author = rhs.author;
	desc.str(rhs.desc.str());
	active = rhs.active;
	return (*this);
}

Manual::Manual()
	: Container()
{}
Manual::Manual(const Manual &rhs)
	: Container(rhs)
{}
Manual::~Manual() throw()
{ Container::clear(); }

Manual						&Manual::operator=(const Manual &rhs)
{
	Container::operator=(rhs);
	return (*this);
}
ManualSection							*Manual::createSection(const String &name_,
															   const String &title_,
															   const StringStream &lines_,
															   const String &author_,
															   const StringStream &desc_) throw (std::runtime_error)
{
	Iterator it(Container::find(name_));
	if (it != Container::end())
		throw std::runtime_error("help_screen '" + name_ + "' already created!");
	std::pair<Iterator, bool> cit(Container::insert(
									  std::make_pair(name_, ManualSection(name_, title_, lines_, author_, desc_))));
	return (&cit.first->second);
}
void								Manual::activateSection(const String &name) throw (std::runtime_error)
{
	Iterator it(Container::find(name));
	if (it == Container::end())
		throw std::runtime_error("Missing help screen: " + name);
	it->second.active = true;
}
void								Manual::disactivateSection(const String &name) throw (std::runtime_error)
{
	Iterator it(Container::find(name));
	if (it == Container::end())
		throw std::runtime_error("Missing help screen: " + name);
	it->second.active = false;
}

ManualSection						*Manual::getActiveSection() throw()
{
	Iterator					it;
	ManualSection				*ret(NULL);
	for (it = Container::begin(); !ret && it != Container::end(); it++)
	{
		if (it->second.active)
			ret = &it->second;
	}
	return (ret);
}
const ManualSection					*Manual::getActiveSection() const throw()
{
	ConstIterator				it;
	const ManualSection			*ret(NULL);
	for (it = Container::begin(); !ret && it != Container::end(); it++)
	{
		if (it->second.active)
			ret = &it->second;
	}
	return (ret);
}

OStream									&operator<<(OStream &os,
													const ManualSection &scr)
{
	const String						hr(80, '-');
	String								line;
	StringStream						lines;
	os << hr << std::endl;
	os << String(1, '\t') << scr.title << ":" << std::endl;
	os << String(1, '\t') << String(scr.title.size() + 1, '-') << std::endl;
	lines.str(scr.desc.str());
	while (std::getline(lines, line))
		os << String(1, '\t') << line << std::endl;
	os << hr << std::endl;
	lines.clear();
	lines.str(scr.lines.str());
	while (std::getline(lines, line))
		os << String(2, '\t') << line << std::endl;
	os << hr;
	return (os);
}
