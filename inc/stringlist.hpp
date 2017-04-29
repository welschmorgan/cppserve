/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringlist.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/10 21:31:28 by mwelsch           #+#    #+#             */
//   Updated: 2017/04/22 16:50:15 by mwelsch          ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#ifndef		STRINGLIST_H
# define	STRINGLIST_H

# include <string>
# include <vector>
# include <map>
# include <memory>
# include "string.hpp"


/**
 * List
 */

template <typename StringT = std::string,
		  template <typename, typename>
		  class ContainerT = std::vector
>
using BasicStringList = ContainerT<StringT, std::allocator<StringT> >;

typedef BasicStringList<std::string>			StringList;
typedef std::shared_ptr<StringList>				SharedStringList;

typedef BasicStringList<std::wstring>			WStringList;
typedef std::shared_ptr<StringList>				SharedWStringList;

#endif
