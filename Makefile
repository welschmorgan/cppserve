#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/04 13:52:33 by mwelsch           #+#    #+#              #
#    Updated: 2017/02/07 21:23:15 by mwelsch          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

TARGET = cppserve

INC_DIR = ./inc
SRC_DIR = ./src
OBJ_DIR = ./obj

SRCS = main.cpp server.cpp launch_options.cpp client.cpp socket.cpp
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(SRCS:%.cpp=%.o))

CXX = clang++
CFLAGS = -I$(INC_DIR) -std=c++11 -g
LDFLAGS = -lstdc++ -stdlib=libstdc++ -std=c++11

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CFLAGS) -o $@ -c $^

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: clean fclean re all
