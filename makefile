#
# Author   : Küenzi Jean-Daniel
# Version  : 1.0.0
# Date     : 23.05.2018
# Update   : 26.05.2018
# Desc.    : Makefile servant a compiler les differentes parties du tp
#

CC = gcc
FLAGS = -g -Wall -Wextra -std=gnu11 -O2
SANITIZERS = -fsanitize=address -fsanitize=leak -fsanitize=undefined
LIBS = -lm -lSDL2 -lpthread

APP=galaxy_simulation

C_LIBS_SRC:=$(shell find ./libs -name "*.c")
C_LIBS_OBJ:=$(C_LIBS_SRC:.c=.o)
C_LIBS_DEPS:=$(C_LIBS_OBJ:.o=.d)

C_MAIN_SRC:=$(wildcard *.c)
C_MAIN_OBJ:=$(C_MAIN_SRC:.c=.o)
C_MAIN_DEPS:=$(C_MAIN_OBJ:.o=.d)

$(APP): $(C_LIBS_OBJ) $(C_MAIN_OBJ) 
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)

all: $(APP) tests

tests: $(C_LIBS_OBJ)
	$(MAKE) -C tests

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: tests

clean:
	$(MAKE) clean -C tests
	rm -rf $(APP) $(C_LIBS_OBJ) $(C_LIBS_DEPS) $(C_MAIN_OBJ) $(C_MAIN_DEPS)