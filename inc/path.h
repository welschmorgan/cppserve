/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 19:19:20 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/14 20:10:26 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		PATH_H
# define	PATH_H

# include <string>
# include <list>
# include "serializer.h"

template<typename T>
bool		PatternMatch(const T &pattern,
						 const T &input);

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
class				BasicPath
	: public BasicSerializer<BasicPath<IStreamT,
									   OStreamT,
									   StringT,
									   SStreamT>,
						IStreamT,
						OStreamT,
						StringT,
						SStreamT>
{
public:
	typedef BasicPath<IStreamT,
					  OStreamT,
					  StringT,
					  SStreamT>				Self;
	typedef BasicSerializer<Self,
							IStreamT,
							OStreamT,
							StringT,
							SStreamT>		Serializer;

	BasicPath(const StringT &full = StringT());
	BasicPath(const Self &);
	virtual			~BasicPath();

	BasicPath		&operator=(const BasicPath &);

	StringT			&set(const StringT &rk);
	StringT			get() const;

	StringT			getDir() const;
	StringT			getBase(bool withExt = true) const;
	StringT			getExt() const;

	StringT			&setDir(const StringT &rk);
	StringT			&setBase(const StringT &rk);
	StringT			&setExt(const StringT &rk);

	OStreamT		&stringify(OStreamT &os) const;
	IStreamT		&parse(IStreamT &is);

	size_t			getSize();

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
	size_t			mSize;
	bool			mSizeFound;
};

typedef BasicPath<std::istream, std::ostream,
				  std::string, std::stringstream>	Path;
typedef BasicPath<std::wistream, std::wostream,
				  std::wstring, std::wstringstream>	WPath;

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
BasicPath<IStreamT,
		  OStreamT,
		  StringT,
		  SStreamT>::BasicPath(const StringT &full)
	: Serializer()
	, mFull(full)
	, mDir()
	, mBase()
	, mExt()
	, mSize(0)
	, mSizeFound(false)
{
	Serializer::fromString(full);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
BasicPath<IStreamT,
		  OStreamT,
		  StringT,
		  SStreamT>::BasicPath(const BasicPath &rk)
	: Serializer()
	, mFull(rk.mFull)
	, mDir(rk.mDir)
	, mBase(rk.mBase)
	, mExt(rk.mExt)
	, mSize(0)
	, mSizeFound(false)
{}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
BasicPath<IStreamT,
		  OStreamT,
		  StringT,
		  SStreamT>::~BasicPath()
{}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
BasicPath<IStreamT,
		  OStreamT,
		  StringT,
		  SStreamT>					&BasicPath<IStreamT,
											   OStreamT,
											   StringT,
											   SStreamT>::operator=(const BasicPath &rk) {
	mFull = rk.mFull;
	mDir = rk.mDir;
	mBase = rk.mBase;
	mExt = rk.mExt;
	mSize = rk.mSize;
	mSizeFound = rk.mSizeFound;
	return (*this);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
size_t			BasicPath<IStreamT,
						   OStreamT,
						   StringT,
						   SStreamT>::getSize() {
	std::cout << "Finding size: " << mFull;
	if (!mSizeFound) {
		std::ifstream ifs(mFull, std::ifstream::binary);
		if (ifs.is_open() && ifs.good()) {
			std::cout << "\topened" << std::endl;
			ifs.seekg(0, std::ios_base::end);
			mSize = ifs.tellg();
			mSizeFound = true;
		} else {
			std:: cout << "\tfailed to open!" << std::endl;
		}
		std:: cout << "-> " << mSize << std::endl;
	}
	return (mSize);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
StringT			&BasicPath<IStreamT,
						   OStreamT,
						   StringT,
						   SStreamT>::set(const StringT &rk) {
	fromString(rk);
	return (mFull);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
StringT			BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::get() const {
	return (operator StringT());
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
StringT			BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::getDir() const {
	return (mDir);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
StringT			BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::getBase(bool withExt) const {
	return (mBase + (withExt ? mExt : std::string()));
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
StringT			BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::getExt() const {
	return (mExt);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
StringT			&BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::setDir(const StringT &rk) {
	mDir = rk;
	return (mDir);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
StringT			&BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::setBase(const StringT &rk) {
	mBase = rk;
	return (mBase);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
StringT			&BasicPath<IStreamT,
						   OStreamT,
						   StringT,
						   SStreamT>::setExt(const StringT &rk) {
	mExt = rk;
	return (mExt);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
OStreamT		&BasicPath<IStreamT,
						   OStreamT,
						   StringT,
						   SStreamT>::stringify(OStreamT &os) const {
	os << mFull;
	return (os);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
IStreamT		&BasicPath<IStreamT,
						   OStreamT,
						   StringT,
						   SStreamT>::parse(IStreamT &is) {
	is >> mFull;
	size_t pos;
	if ((pos = mFull.find_last_of('/')) != std::string::npos) {
		mDir = mFull.substr(0, pos);
		mBase = mFull.substr(pos + 1);
	} else {
		mDir = "";
		mBase = mFull;
	}
	if ((pos = mBase.find_first_of('.')) != std::string::npos) {
		mExt = mBase.substr(pos);
		mBase = mBase.substr(0, pos);
	} else {
		mExt = "";
	}
	return (is);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool						BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator!=(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator==(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator>(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator<(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator>=(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator<=(const BasicPath &rk)
{
	bool					ret(false);
	return (ret);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator==(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator!=(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
								  OStreamT,
								  StringT,
								  SStreamT>::operator>(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator<(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator>=(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}
template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
bool					BasicPath<IStreamT,
						  OStreamT,
						  StringT,
						  SStreamT>::operator<=(const StringT &rk)
{
	bool					ret(false);
	return (ret);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
typename StringT::value_type &BasicPath<IStreamT,
								  OStreamT,
								  StringT,
								  SStreamT>::operator[](size_t offset) {
	return (at(offset));
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
typename StringT::value_type			BasicPath<IStreamT,
								  OStreamT,
								  StringT,
								  SStreamT>::at(size_t offset) const {
	return (mFull.at(offset));
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
typename StringT::value_type			&BasicPath<IStreamT,
								  OStreamT,
								  StringT,
								  SStreamT>::at(size_t offset) {
	return (mFull.at(offset));
}

template<typename T>
bool		PatternMatch(const T &pattern,
						 const T &input) {
	typename T::const_iterator	a(pattern.begin());
	typename T::const_iterator	nexta;
	typename T::const_iterator	b(input.begin());
	bool						ret(true);
	if (pattern != T("*")) {
		while (ret
			   && a != pattern.end()
			   && b != input.end()) {
			if (*a == '*') {
				nexta = a + 1;
				while (b != input.end()) {
					if (nexta == pattern.end()
						|| *nexta == *b)
						break ;
					b++;
				}
				if (nexta == pattern.end())
					break ;
				a++;
			} else if (*a != *b) {
				ret = false;
			}
			a++;
			b++;
		}
		if (a != pattern.end() || b != input.end())
			ret = false;
	}
	return (ret);
}

template<typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT,
		 template <typename, typename>
		 class ContainerT = std::list,
		 template <typename>
		 class AllocT = std::allocator>
class PathList
	: public ContainerT<BasicPath<IStreamT,
								  OStreamT,
								  StringT,
								  SStreamT>,
						AllocT<BasicPath<IStreamT,
								  OStreamT,
								  StringT,
								  SStreamT> > >
{
public:
	typedef ContainerT<BasicPath<IStreamT,
								  OStreamT,
								  StringT,
								  SStreamT>,
					   AllocT<BasicPath<IStreamT,
								  OStreamT,
								  StringT,
								  SStreamT> > > Container;
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
