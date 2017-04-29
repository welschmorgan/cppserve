#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/04/21 23:00:36 by mwelsch           #+#    #+#              #
#    Updated: 2017/04/29 19:02:03 by mwelsch          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

TESTS = $(shell find . -mindepth 1 -maxdepth 1 -type d | sed 's/\.\///')

MAKE=make --no-print-directory

define work
	for FILE in $(TESTS); do \
		TARGET=$$FILE $(MAKE) -C $$FILE -f ../unit.make $(1); \
	done
endef

all:
	@$(call work,all)

run:
	@$(call work,run)

clean:
	@$(call work,clean)

fclean:
	@$(call work,fclean)

re: fclean all

.PHONY: clean fclean re all run
