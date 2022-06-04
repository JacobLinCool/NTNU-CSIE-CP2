CC = gcc
FLAGS = -lm
PROD_FLAGS = -O0
DEV_FLAGS = -fsanitize=address -Wall -Wextra -g3 -D DEBUG

SRCS = $(wildcard fin*.c)
OUTS = $(patsubst fin%.c,fin%,$(SRCS))

all:
	@make build -j

build: FLAGS += $(PROD_FLAGS)
build: $(OUTS)
	@echo "Targets built in production mode."
	@echo "    flags: \"$(FLAGS)\""

dev: FLAGS += $(DEV_FLAGS)
dev: $(OUTS)
	@echo "Targets built in development mode."
	@echo "    flags: \"$(FLAGS)\""

fin01: force
	@$(CC) $(FLAGS) -o $@ $@.c
fin02: force
	@$(CC) $(FLAGS) -o $@ $@.c
fin03: force maze.o
	@$(CC) $(FLAGS) -o $@ $@.c maze.o

maze.o:
	@$(CC) $(FLAGS) -c maze.c

clean:
	@rm -f $(OUTS) maze.o *.zip
	@echo "Cleaned"

zip: 
	zip -r fin *.c *.h *.pdf Makefile lib

list:
	@echo "SRCS: $(SRCS)"
	@echo "OUTS: $(OUTS)"

.PHONY: all build dev clean zip list force
