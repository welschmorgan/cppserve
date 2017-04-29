/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringmap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 16:15:02 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/22 16:50:21 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		STRINGMAP_H
# define	STRINGMAP_H

# include <string>
# include <map>
# include <memory>
# include "string.hpp"

/**
 * Map
 */
template<typename StringT = std::string,
		 template <typename, typename, typename, typename>
		 class ContainerT = std::map,
		 template <typename>
		 class ComparT = std::less,
		 template <typename T>
		 class AllocT = std::allocator>
using BasicStringMap = ContainerT<StringT,
								  StringT,
								  ComparT<StringT>,
								  AllocT<StringT> >;

typedef BasicStringMap<std::string>					StringMap;
typedef std::shared_ptr<StringMap>					SharedStringMap;

typedef BasicStringMap<std::wstring>				WStringMap;
typedef std::shared_ptr<StringMap>					SharedWStringMap;

#endif
