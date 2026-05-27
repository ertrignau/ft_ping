# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eric <eric@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/21 00:00:00 by ertrigna          #+#    #+#              #
#    Updated: 2026/05/27 18:59:08 by eric             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ft_ping

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -g
INCLUDES	= -I inc

SRCS_DIR	= srcs
OBJ_DIR		= obj
SRCS		= $(SRCS_DIR)/main.c \
			  $(SRCS_DIR)/init.c \
			  $(SRCS_DIR)/parsing.c \
			  $(SRCS_DIR)/flags.c \
			  $(SRCS_DIR)/ping.c \
			  $(SRCS_DIR)/utils.c \
			  $(SRCS_DIR)/icmp.c

OBJS		= $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))

all: banner $(NAME)

banner:
	@echo  "\033[32m╔═════════════════════════════════════════════════════════════════════╗\033[0m"
	@echo  "\033[32m║                                                                     ║\033[0m"
	@echo  "\033[32m║           ███████╗████████╗    ██████╗ ██╗███╗   ██╗ ██████╗        ║\033[0m"
	@echo  "\033[32m║           ██╔════╝╚══██╔══╝    ██╔══██╗██║████╗  ██║██╔════╝        ║\033[0m"
	@echo  "\033[32m║           █████╗     ██║       ██████╔╝██║██╔██╗ ██║██║  ███╗       ║\033[0m"
	@echo  "\033[32m║           ██╔══╝     ██║       ██╔═══╝ ██║██║╚██╗██║██║   ██║       ║\033[0m"
	@echo  "\033[32m║           ██║        ██║       ██║     ██║██║ ╚████║╚██████╔╝       ║\033[0m"
	@echo  "\033[32m║           ╚═╝        ╚═╝       ╚═╝     ╚═╝╚═╝  ╚═══╝ ╚═════╝        ║\033[0m"
	@echo  "\033[32m║                                                                     ║\033[0m"
	@echo  "\033[32m║                    ICMP Network Diagnostic Tool                     ║\033[0m"
	@echo  "\033[32m║                                                                     ║\033[0m"
	@echo  "\033[32m╚═════════════════════════════════════════════════════════════════════╝\033[0m"
	@echo ""

$(NAME): $(OBJ_DIR) $(OBJS)
	@printf "\n"
	@printf "  ⏳ Linking binary... "
	@for i in $$(seq 1 30); do printf "█"; sleep 0.01; done
	@printf "\n"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lm
	@echo ""
	@echo "✅ Binary compiled: ./$(NAME)"
	@echo "   Usage: sudo ./$(NAME) [options] <destination>"
	@echo ""

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.c
	@printf "  %-60s\r" "🔨 Compiling $*..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 2>&1 | sed 's/^/    /' || (echo ""; exit 1)
	@printf "  %-60s\r" "✓ Compiled $*"

test: $(NAME)
	@echo "\033[32m╔═══════════════════════════════════════════════════════════════╗\033[0m"
	@echo "\033[32m║                   Running Test Suite                          ║\033[0m"
	@echo "\033[32m╚═══════════════════════════════════════════════════════════════╝\033[0m"
	@echo ""
	@echo "📋 Parsing & Flags validation..."
	@./script/test_flags.sh
	@echo ""
	@echo "🔍 Valgrind memory leak detection (flags)..."
	@./script/test_flags_valgrind.sh 127.0.0.1
	@echo ""
	@echo "🔍 Valgrind parser detection..."
	@./script/valgrind_parser.sh 127.0.0.1
	@echo ""
	@echo "⏱️  Performance & Timestamp comparison..."
	@./script/compare_ping_timestamp.sh 127.0.0.1
	@echo ""
	@echo "\033[32m╔═══════════════════════════════════════════════════════════════╗\033[0m"
	@echo "\033[32m║                    ✅ All tests passed!                       ║\033[0m"
	@echo "\033[32m╚═══════════════════════════════════════════════════════════════╝\033[0m"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "🧹 Object files cleaned"

fclean: clean
	@rm -f $(NAME)
	@echo "🧹 Binary cleaned"

re: fclean all

help:
	@echo ""
	@echo "╔═══════════════════════════════════════════════════════════════╗"
	@echo "║                   Available Targets                           ║"
	@echo "╚═══════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "  make              → Compile ft_ping"
	@echo "  make clean        → Remove object files"
	@echo "  make fclean       → Remove everything (objects + binary)"
	@echo "  make re           → Clean and rebuild"
	@echo "  make test         → Run all tests (parsing, valgrind, perf)"
	@echo "  make help         → Show this help message"
	@echo ""

.PHONY: all clean fclean re test help banner
