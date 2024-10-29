CC = gcc
SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(patsubst src/%, bin/obj/%, $(SRC:.c=.o))
NAME = buk
EXEC = bin/$(NAME)
INSTALL_PATH = /usr/local/bin

RESET = \033[0m
YELLOW = \033[0;33m
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[1;34m

all: clean compile

compile:
	mkdir -p $(dir $(OBJ))
	$(foreach file, $(SRC), \
		$(CC) -c $(file) -o $(patsubst src/%, bin/obj/%, $(file:.c=.o)) &&) true
	$(CC) $(OBJ) -o $(EXEC)
	echo "$(YELLOW)•$(RESET) Compiled"

clean:
	rm -rf bin
	echo "$(YELLOW)•$(RESET) Removed executable and objects"

install: all
	sudo cp $(EXEC) $(INSTALL_PATH)
	echo "$(GREEN)•$(RESET) Installed \033[1;32m$(NAME)$(RESET) to $(BLUE)$(INSTALL_PATH)$(RESET)"

uninstall:
	sudo rm -f $(INSTALL_PATH)/$(EXEC)
	echo "$(RED)•$(RESET) Uninstalled \033[1;32m$(NAME)$(RESET) from $(BLUE)$(INSTALL_PATH)$(RESET)"

.PHONY: all compile clean install uninstall
.SILENT: all compile clean install uninstall