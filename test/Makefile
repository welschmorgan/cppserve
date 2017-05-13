#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/04/21 23:00:36 by mwelsch           #+#    #+#              #
#    Updated: 2017/05/13 15:21:46 by mwelsch          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

TESTS = $(shell find . -mindepth 1 -maxdepth 1 -type d | sed 's/\.\///' | grep -v '_suite')

MAKE=make --no-print-directory

define work
	for FILE in $(TESTS); do \
		echo "$${FILE}/$${FILE}" >> '.gitignore'; \
		TARGET=$$FILE $(MAKE) -C $$FILE -f ../unit.make $(1); \
	done
endef

all:
	@rm -f '.gitignore'
	@touch '.gitignore'
	TARGET=suite $(MAKE) -C _suite -f ../unit.make $(1)
	@$(call work,all)

suite:
	TARGET=suite $(MAKE) -C _suite -f ../unit.make $(1)

run:
#TARGET=suite $(MAKE) run -C _suite -f ../unit.make $(1)
	@$(call work,run)

clean:
	@$(call work,clean)
	TARGET=suite $(MAKE) clean -C _suite -f ../unit.make $(1)

fclean:
	@$(call work,fclean)
	TARGET=suite $(MAKE) fclean -C _suite -f ../unit.make $(1)

re: fclean all

.PHONY: clean fclean re all run
