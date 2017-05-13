SERV_DIR=../..

SERV_INC_DIR=$(SERV_DIR)/inc
SERV_OBJ_DIR=$(SERV_DIR)/obj
SERV_SRC_DIR=$(SERV_DIR)/src

INC_DIR=inc
OBJ_DIR=$(SERV_OBJ_DIR)/lib/$(TARGET)
SRC_DIR=src

CC=clang++
LD=ar rcs
CXXFLAGS=-std=c++11 -stdlib=libstdc++ -ggdb -I. -I$(INC_DIR) -I$(SERV_DIR)/$(INC_DIR) -Wall -Werror -Wextra
LDFLAGS=#-lstdc++ #-L$(SERV_DIR) -lunit_test

SRCS=$(shell test -e $(SRC_DIR) && find $(SRC_DIR) -type f | grep '.cpp')
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

PLAIN_SRCS=$(shell find . -maxdepth 1 -mindepth 1 -type f | grep '.cpp')
PLAIN_OBJS=$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(PLAIN_SRCS))

all: lib$(TARGET).a

lib$(TARGET).a: $(OBJS) $(PLAIN_OBJS)
	$(LD) $(LDFLAGS) $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	@if [ ! -z "$(OBJS)$(PLAIN_OBJS)" ]; then \
		rm -f $(OBJS) $(PLAIN_OBJS); \
	fi

fclean: clean
	rm -f lib$(TARGET).a

re: fclean all


.PHONY: clean fclean re all
