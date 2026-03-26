# Host type detection
ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME			= libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

# Compiler and flags
CC				= cc
CFLAGS			= -Wall -Wextra -Werror -fPIC
INCLUDES		= -I./includes

# Directories
SRC_DIR			= src
OBJ_DIR			= obj

# Source files
SRCS			= $(SRC_DIR)/malloc/malloc.c \
					  $(SRC_DIR)/utils/ft_memcpy.c \
					  $(SRC_DIR)/utils/ft_memset.c \
					  $(SRC_DIR)/utils/ft_bzero.c
# Object files
OBJS			= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Colors
GREEN			= \033[0;32m
RED				= \033[0;31m
RESET			= \033[0m

# Docker
DOCKER_PLATFORM ?= linux/amd64

# Rules
all: $(NAME) symlink

$(NAME): $(OBJS)
	@echo "$(GREEN)Creating $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -shared -o $(NAME)
	@echo "$(GREEN)$(NAME) created successfully$(RESET)"

symlink: $(NAME)
	@ln -sf $(NAME) $(LINK)
	@echo "$(GREEN)Symlink $(LINK) created for $(NAME)$(RESET)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME) $(LINK)

re: fclean all

docker-build:
	@DOCKER_DEFAULT_PLATFORM=$(DOCKER_PLATFORM) docker compose build

docker-up:
	@DOCKER_DEFAULT_PLATFORM=$(DOCKER_PLATFORM) docker compose up -d

docker-down:
	@DOCKER_DEFAULT_PLATFORM=$(DOCKER_PLATFORM) docker compose down

docker-shell:
	@DOCKER_DEFAULT_PLATFORM=$(DOCKER_PLATFORM) docker compose exec malloc /bin/bash

docker-rebuild: docker-down docker-build docker-up

.PHONY: all symlink clean fclean re
