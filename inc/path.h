/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 19:19:20 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/12 21:46:36 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		PATH_H
# define	PATH_H

# include <string>
# include <list>
# include "serializer.h"

template<typename StringT>
class				BasicPath
	: public Serializer<BasicPath<StringT>, StringT>
{
public:
	BasicPath(const StringT &name);
	BasicPath(const BasicPath &);
	virtual			~BasicPath();

	BasicPath		&operator=(const BasicPath &);

	StringT			&set(const StringT &rk);
	StringT			get() const;

	StringT			getDir() const;
	StringT			getBase() const;
	StringT			getExt() const;

	StringT			&setDir(const StringT &rk);
	StringT			&setBase(const StringT &rk);
	StringT			&setExt(const StringT &rk);

	std::ostream	&stringify(std::ostream &os) const;
	std::istream	&parse(std::istream &is);

	bool			operator!=(const BasicPath &rk);
	bool			operator==(const BasicPath &rk);
	bool			operator>(const BasicPath &rk);
	bool			operator<(const BasicPath &rk);
	bool			operator>=(const BasicPath &rk);
	bool			operator<=(const BasicPath &rk);

	bool			operator==(const StringT &rk);
	bool			operator!=(const StringT &rk);
	bool			operator>(const StringT &rk);
	bool			operator<(const StringT &rk);
	bool			operator>=(const StringT &rk);
	bool			operator<=(const StringT &rk);

	typename StringT::value_type		&operator[](size_t offset);
	typename StringT::value_type		at(size_t offset = 0) const;
	typename StringT::value_type		&at(size_t offset = 0);
protected:
	StringT			mFull;
	StringT			mDir;
	StringT			mBase;
	StringT			mExt;
};

typedef BasicPath<std::string> Path;
typedef BasicPath<std::wstring> WPath;

template<typename StringT>
BasicPath<StringT>::BasicPath(const StringT &full)
	: mFull()
	, mDir()
	, mBase()
	, mExt()
{
	parse(full);
}

template<typename StringT>
BasicPath<StringT>::BasicPath(const BasicPath &rk)
	: mFull(rk.mFull)
	, mDir(rk.mDir)
	, mBase(rk.mBase)
	, mExt(rk.mExt)
{}

template<typename StringT>
BasicPath<StringT>::~BasicPath()
{}
template<typename StringT>
BasicPath<StringT>					&BasicPath<StringT>::operator=(const BasicPath &rk) {
	mFull = rk.mFull;
	mDir = rk.mDir;
	mBase = rk.mBase;
	mExt = rk.mExt;
	return (*this);
}

template<typename StringT>
StringT			&BasicPath<StringT>::set(const StringT &rk) {
	parse(rk);
	return (mFull);
}

template<typename StringT>
StringT			BasicPath<StringT>::get() const {
	return (operator StringT());
}

template<typename StringT>
StringT			BasicPath<StringT>::getDir() const {
	return (mDir);
}

template<typename StringT>
StringT			BasicPath<StringT>::getBase() const {
	return (mBase);
}

template<typename StringT>
StringT			BasicPath<StringT>::getExt() const {
	return (mExt);
}

template<typename StringT>
StringT			&BasicPath<StringT>::setDir(const StringT &rk) {
	mDir = rk;
	return (mDir);
}

template<typename StringT>
StringT			&BasicPath<StringT>::setBase(const StringT &rk) {
	mBase = rk;
	return (mBase);
}

template<typename StringT>
StringT			&BasicPath<StringT>::setExt(const StringT &rk) {
	mExt = rk;
	return (mExt);
}

template<typename StringT>
std::ostream				&BasicPath<StringT>::stringify(std::ostream &os) const {
	if (os)
		os << mFull;
	return (os);
}

template<typename StringT>
std::istream				&BasicPath<StringT>::parse(std::istream &is) {
	if (is)
		is >> mFull;
	return (is);
}

template<typename StringT>
bool						BasicPath<StringT>::operator!=(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename StringT>
bool					BasicPath<StringT>::operator==(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename StringT>
bool					BasicPath<StringT>::operator>(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename StringT>
bool					BasicPath<StringT>::operator<(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename StringT>
bool					BasicPath<StringT>::operator>=(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename StringT>
bool					BasicPath<StringT>::operator<=(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}

template<typename StringT>
bool					BasicPath<StringT>::operator==(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename StringT>
bool					BasicPath<StringT>::operator!=(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename StringT>
bool					BasicPath<StringT>::operator>(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename StringT>
bool					BasicPath<StringT>::operator<(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename StringT>
bool					BasicPath<StringT>::operator>=(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename StringT>
bool					BasicPath<StringT>::operator<=(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}

template<typename StringT>
typename StringT::value_type &BasicPath<StringT>::operator[](size_t offset) {
	return (at(offset));
}

template<typename StringT>
typename StringT::value_type			BasicPath<StringT>::at(size_t offset) const {
	return (mFull.at(offset));
}

template<typename StringT>
typename StringT::value_type			&BasicPath<StringT>::at(size_t offset) {
	return (mFull.at(offset));
}


template<typename StringT,
		 template <typename, typename>
		 class ContainerT = std::list,
		 template <typename>
		 class AllocT = std::allocator>
class PathList
	: public ContainerT<BasicPath<StringT>,
						AllocT<BasicPath<StringT> > >
{
public:
	typedef ContainerT<BasicPath<StringT>,
					   AllocT<BasicPath<StringT> > > Container;
	typedef typename Container::iterator iterator;
	typedef typename Container::const_iterator const_iterator;
	typedef typename Container::reverse_iterator reverse_iterator;
	typedef typename Container::const_reverse_iterator const_reverse_iterator;

	PathList()
		: Container()
		{}
	PathList(const PathList &rk)
		: Container(rk)
		{}
	~PathList()
		{}

	StringT		join(const StringT &sep) const {
		const_iterator it;
		StringT	ret;
		for (it = Container::begin(); it != Container::end(); it++) {
			if (!ret.empty())
				ret += sep;
			ret += it->get();
		}
		return (ret);
	}
	PathList &operator=(const PathList &rk) {
		Container::operator=(rk);
		return (*this);
	}
};

#endif
