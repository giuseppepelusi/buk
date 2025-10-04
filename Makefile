CC = gcc
SRC = $(wildcard src/*.c src/*/*.c)
OBJ = $(patsubst src/%, bin/obj/%, $(SRC:.c=.o))
NAME = buk
EXEC = bin/$(NAME)
INSTALL_PATH = /usr/local/bin
MAN = $(NAME).1
MAN_INSTALL_PATH = /usr/local/share/man/man1

RESET = \033[0m
YELLOW = \033[0;33m
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[1;34m

all: compile

bin/obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) -c $< -o $@

compile: $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIB)
	echo "$(YELLOW)•$(RESET) Compiled"

clear:
	rm -rf bin
	echo "$(YELLOW)•$(RESET) Removed executable and objects"

install: compile install-man
	sudo install -Dm755 $(EXEC) $(INSTALL_PATH)
	echo "$(GREEN)•$(RESET) Installed \033[1;32m$(NAME)$(RESET) to $(BLUE)$(INSTALL_PATH)$(RESET)"

install-man:
	sudo mkdir -p $(MAN_INSTALL_PATH)
	sudo cp doc/man/$(MAN) $(MAN_INSTALL_PATH)
	sudo mandb >/dev/null 2>&1

uninstall-man:
	sudo rm -f $(MAN_INSTALL_PATH)/$(MAN)

uninstall: uninstall-man
	sudo rm -f $(INSTALL_PATH)/$(NAME)
	echo "$(RED)•$(RESET) Uninstalled \033[1;32m$(NAME)$(RESET) from $(BLUE)$(INSTALL_PATH)$(RESET)"

.PHONY: all compile clear install install-man uninstall-man
.SILENT: all compile clear install install-man uninstall uninstall-man
