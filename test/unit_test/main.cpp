// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/23 18:28:28 by mwelsch           #+#    #+#             //
//   Updated: 2017/04/24 21:47:03 by mwelsch          ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

# include <iostream>
# include <stdlib.h>
# include "unit_test.hpp"

class		type
{
public:
	static int		run_static(void*) {
		std::cout << "static" << std::endl;
		return (0);
	};
	int				run_member(void*) {
		std::cout << "member" << std::endl;
		return (0);
	};
};

# include <vector>

int			main()
{
	unit_test::func<int, void*>("static function",
								type::run_static)
		.must_return(10, NULL);
	return (EXIT_SUCCESS);
}
