ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIC
INCLUDES = -I./includes

SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests
TEST_BIN = $(TEST_DIR)/test_malloc
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c) $(wildcard $(TEST_DIR)/malloc/*.c) \
	$(wildcard $(TEST_DIR)/free/*.c)

SRCS = $(SRC_DIR)/malloc.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/block.c \
	$(SRC_DIR)/heap.c \
	$(SRC_DIR)/free.c \
	$(SRC_DIR)/show_alloc_mem.c \
	

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

DOCKER_PLATFORM ?= linux/amd64

all: $(NAME) symlink

test: fclean $(NAME) symlink $(TEST_BIN)
	@echo "$(GREEN)Running tests with $(LINK)...$(RESET)"
	@printf "\n$(GREEN)================ TEST RUN ================\n$(RESET)"
	@DYLD_INSERT_LIBRARIES=./$(LINK) DYLD_FORCE_FLAT_NAMESPACE=1 ./$(TEST_BIN)
	@printf "$(GREEN)==========================================\n$(RESET)"

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

$(TEST_BIN): $(TEST_SRCS) $(LINK)
	@echo "Compiling test sources..."
	@$(CC) -Wall -Wextra -Werror $(INCLUDES) $(TEST_SRCS) ./$(LINK) -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)Removing build artifacts...$(RESET)"
	@rm -f $(NAME) $(LINK) $(TEST_BIN)

re: fclean all

docker-build:
	@DOCKER_DEFAULT_PLATFORM=$(DOCKER_PLATFORM) docker compose build

docker-up:
	@DOCKER_DEFAULT_PLATFORM=$(DOCKER_PLATFORM) docker compose up -d

docker-down:
	@DOCKER_DEFAULT_PLATFORM=$(DOCKER_PLATFORM) docker compose down

docker-shell:
	@DOCKER_DEFAULT_PLATFORM=$(DOCKER_PLATFORM) docker compose exec malloc /bin/bash

.PHONY: all symlink test clean fclean re docker-build docker-up docker-down docker-shell docker-rebuild
