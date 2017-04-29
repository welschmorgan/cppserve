#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/04/21 23:00:36 by mwelsch           #+#    #+#              #
#    Updated: 2017/04/23 18:22:51 by mwelsch          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

LIBS = $(shell find . -mindepth 1 -maxdepth 1 -type d | sed 's/\.\///')

MAKE=make --no-print-directory

define work
	for LIB in $(LIBS); do \
		TARGET=$$LIB $(MAKE) -C $$LIB -f ../lib.make $(1); \
	done
endef

all:
	@$(call work,all)

clean:
	@$(call work,clean)

fclean:
	@$(call work,fclean)

re: fclean all

.PHONY: clean fclean re all
