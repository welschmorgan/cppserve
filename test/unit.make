SERV_DIR=../..

SERV_INC_DIR=$(SERV_DIR)/inc
SERV_OBJ_DIR=$(SERV_DIR)/obj
SERV_LIB_DIR=$(SERV_DIR)/lib
SERV_SRC_DIR=$(SERV_DIR)/src

LIBS=$(shell find $(SERV_LIB_DIR) -mindepth 1 -maxdepth 1 -type d)
INC_DIR=inc
OBJ_DIR=$(SERV_OBJ_DIR)/unit_test/$(TARGET)
SRC_DIR=src

CC=clang++
STDLIB_FLAGS=-std=c++11 -stdlib=libstdc++
CXXFLAGS=$(STDLIB_FLAGS) -ggdb -I. -I$(INC_DIR) $(patsubst %,-I%,$(LIBS)) $(patsubst %,-I%/inc,$(LIBS)) -I$(SERV_DIR)/$(INC_DIR) -Wall -Werror -Wextra
LDFLAGS=$(STDLIB_FLAGS) -lstdc++ -L$(SERV_LIB_DIR) $(patsubst %,-L%,$(LIBS)) $(patsubst %,-l%,$(shell echo $(LIBS) | xargs basename))
#-L$(SERV_DIR) -lunit_test

SRCS=$(shell test -e $(SRC_DIR) && find $(SRC_DIR) -type f | grep '.cpp')
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

PLAIN_SRCS=$(shell find . -maxdepth 1 -mindepth 1 -type f | grep '.cpp')
PLAIN_OBJS=$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(PLAIN_SRCS))

SERV_SRCS=$(shell find $(SERV_SRC_DIR) -type f | grep '.cpp' | grep -v 'main.cpp')
SERV_OBJS=$(patsubst $(SERV_SRC_DIR)/%.cpp,$(SERV_OBJ_DIR)/%.o,$(SERV_SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) $(PLAIN_OBJS) $(SERV_OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $^

$(SERV_OBJ_DIR)/%.o: $(SERV_SRC_DIR)/%.cpp
	make -C $(SERV_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(PLAIN_OBJS)

fclean: clean
	rm -f $(TARGET)

run: $(TARGET)
	@printf "[==================== TEST: $(TARGET) =====================]\n" "=" "="
	@./$^ && (printf "\t\t->\033[0;32mOK\033[0m\n"; exit 0) || (printf "\t\t->\033[0;31mKO\033[0m\n"; exit 1)

re: fclean all


.PHONY: clean fclean re all run
