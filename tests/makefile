#
# Author   : Küenzi Jean-Daniel
# Version  : 1.0.0
# Date     : 23.05.2018
# Update   : 26.05.2018
# Desc.    : Makefile servant a compiler les tests
#

CC = gcc
FLAGS = -g -Wall -Wextra -std=gnu11
SANITIZERS = -fsanitize=address -fsanitize=leak -fsanitize=undefined
LIBS = -lm

C_VECTORS_OBJ:=$(shell find ../libs/vectors -name "*.o")
C_STARS_OBJ:=$(shell find ../libs/stars -name "*.o")
C_BOX_OBJ:=$(shell find ../libs/box -name "*.o")

TEST_VECTORS=test_vectors
TEST_BOX=test_box
TEST_STARS=test_stars

all: $(TEST_VECTORS) $(TEST_BOX) $(TEST_STARS)

$(TEST_VECTORS): $(TEST_VECTORS).o $(C_VECTORS_OBJ) $(C_VECTORS_OBJ)
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)

$(TEST_BOX): $(TEST_BOX).o $(C_BOX_OBJ) $(C_VECTORS_OBJ)
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)

$(TEST_STARS): $(TEST_STARS).o $(C_STARS_OBJ) $(C_VECTORS_OBJ) $(C_BOX_OBJ)
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)

%.o: %.c %.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TEST_BOX) $(TEST_STARS) $(TEST_VECTORS)