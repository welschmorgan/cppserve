// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2017/04/23 18:28:28 by mwelsch           #+#    #+#             //
//   Updated: 2017/05/13 15:26:41 by mwelsch          ###   ########.fr       //
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
	unit_test::func<int, void*> fn("static function",
								   "simple static func test",
								   type::run_static);
	if (fn(NULL) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
