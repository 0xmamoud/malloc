NAME			= malloc

# Compiler and flags
CC				= cc
CFLAGS			= -Wall -Wextra -Werror

# Directories
OBJ_DIR			= obj

# Source files
SRCS			= main.c

# Object files
OBJS			= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Colors
GREEN			= \033[0;32m
RED				= \033[0;31m
RESET			= \033[0m

# Docker
DOCKER_PLATFORM ?= linux/amd64

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(GREEN)Creating $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) created successfully$(RESET)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)

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

.PHONY: all clean fclean re docker-build docker-up docker-down docker-shell docker-rebuild
