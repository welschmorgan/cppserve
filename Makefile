#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/04 13:52:33 by mwelsch           #+#    #+#              #
#    Updated: 2017/04/23 18:27:03 by mwelsch          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

TARGET = cppserve

INC_DIR = inc
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj

LIBS = process

SRCS = main.cpp host.cpp server.cpp launch_options.cpp client.cpp socket.cpp \
address.cpp manual.cpp locator.cpp access_control.cpp request.cpp response.cpp \
status.cpp protocol.cpp cache.cpp logger.cpp
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(SRCS:%.cpp=%.o))

MAKE = make --no-print-directory
CC = g++

STDLIB_FLAGS = -std=c++11 #-stdlib=libstdc++
CXXFLAGS = $(STDLIB_FLAGS) -I$(INC_DIR) -g -ggdb -Wall -Werror -Wextra
LDFLAGS = $(STDLIB_FLAGS) -lstdc++ -lc  -L $(LIB_DIR) $(patsubst %,-L%,$(LIB_DIR)/$(LIBS)) $(patsubst %,-l%,$(LIBS))

all: $(TARGET) test

test:
	$(MAKE) -C test

libs:
	$(MAKE) -C lib

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@test -e $(OBJ_DIR) || mkdir -p $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c -o $@ $^ -I$(INC_DIR) -I$(INC_DIR)/logger

clean:
	rm -f $(OBJS)
	$(MAKE) -C test clean

fclean: clean
	rm -f $(TARGET)
	$(MAKE) -C test fclean

re: fclean all

.PHONY: clean fclean re all test libs
