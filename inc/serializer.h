/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serializer.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 18:29:24 by mwelsch           #+#    #+#             */
//   Updated: 2017/02/12 21:13:09 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		SERIALIZER_H
# define	SERIALIZER_H

# include <string>
# include <iostream>
# include <fstream>
# include <sstream>

template <typename T,
		  typename IStreamT,
		  typename OStreamT,
		  typename StringT = std::string,
		  typename SStreamT = std::stringstream>
class BasicSerializer {
private:
	BasicSerializer(const BasicSerializer &rk);
	BasicSerializer &operator=(const BasicSerializer &rk);
public:
	BasicSerializer() {}
	virtual ~BasicSerializer() {}

	virtual OStreamT				&stringify(OStreamT &os) const = 0;
	virtual IStreamT				&parse(IStreamT &is) = 0;

	operator						StringT() const;

	friend OStreamT					&operator<<(OStreamT &os,
												const BasicSerializer<T, IStreamT, OStreamT> &c);
	friend IStreamT					&operator>>(IStreamT &is,
												BasicSerializer<T, IStreamT, OStreamT> &c);
};

template<typename T,
		 typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
OStreamT					&operator<<(OStreamT &os,
										const BasicSerializer<T, IStreamT, OStreamT, StringT, SStreamT> &c) {
	return (c.stringify(os));
}

template<typename T,
		 typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
IStreamT					&operator>>(IStreamT &is,
										BasicSerializer<T, IStreamT, OStreamT, StringT, SStreamT> &c) {
	return (c.parse(c));
}

template<typename T,
		 typename IStreamT,
		 typename OStreamT,
		 typename StringT,
		 typename SStreamT>
BasicSerializer<T, IStreamT, OStreamT, StringT, SStreamT>::operator		StringT() const {
	SStreamT					ss;
	stringify(ss);
	return (ss.str());
		}


template<typename T,
		 typename IStreamT = std::istream,
		 typename OStreamT = std::ostream,
		 typename StringT = std::string,
		 typename SStreamT = std::stringstream>
using Serializer = BasicSerializer<T, IStreamT, OStreamT, StringT, SStreamT>;

template<typename T,
		 typename IStreamT = std::wistream,
		 typename OStreamT = std::wostream,
		 typename StringT = std::wstring,
		 typename SStreamT = std::wstringstream>
using WSerializer = BasicSerializer<T, IStreamT, OStreamT, StringT, SStreamT>;


template<typename T>
using FileSerializer = Serializer<T, std::ifstream, std::ofstream>;
template<typename T>
using WFileSerializer = WSerializer<T, std::wifstream, std::wofstream>;

#endif
