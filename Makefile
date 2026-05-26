# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eric <eric@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/21 00:00:00 by ertrigna          #+#    #+#              #
#    Updated: 2026/01/26 12:32:04 by eric             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ft_ping

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -g
INCLUDES	= -I inc

SRCS_DIR	= srcs
SRCS		= $(SRCS_DIR)/main.c \
			  $(SRCS_DIR)/init.c \
			  $(SRCS_DIR)/parsing.c \
			  $(SRCS_DIR)/ping.c \
			  $(SRCS_DIR)/utils.c \
			  $(SRCS_DIR)/icmp.c

OBJS		= $(SRCS:.c=.o)

all: banner $(NAME)

banner:
	@echo "╔═════════════════════════════════════════════════════════════════════╗"
	@echo "║                                                                     ║"
	@echo "║           ███████╗████████╗    ██████╗ ██╗███╗   ██╗ ██████╗        ║"
	@echo "║           ██╔════╝╚══██╔══╝    ██╔══██╗██║████╗  ██║██╔════╝        ║"
	@echo "║           █████╗     ██║       ██████╔╝██║██╔██╗ ██║██║  ███╗       ║"
	@echo "║           ██╔══╝     ██║       ██╔═══╝ ██║██║╚██╗██║██║   ██║       ║"
	@echo "║           ██║        ██║       ██║     ██║██║ ╚████║╚██████╔╝       ║"
	@echo "║           ╚═╝        ╚═╝       ╚═╝     ╚═╝╚═╝  ╚═══╝ ╚═════╝        ║"
	@echo "║                                                                     ║"
	@echo "║                    ICMP Network Diagnostic Tool                     ║"
	@echo "║                                                                     ║"
	@echo "╚═════════════════════════════════════════════════════════════════════╝"
	@echo ""

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lm
	@echo ""
	@echo "✅ Binary compiled: ./$(NAME)"
	@echo "   Usage: sudo ./$(NAME) [options] <destination>"
	@echo ""

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: $(NAME)
	@echo "╔═══════════════════════════════════════════════════════════════╗"
	@echo "║                   Running Test Suite                          ║"
	@echo "╚═══════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "📋 Parsing & Flags validation..."
	@./script/test_flags.sh
	@echo ""
	@echo "🔍 Valgrind memory leak detection..."
	@./script/test_flags_valgrind.sh 127.0.0.1
	@echo ""
	@echo "⚡ Performance benchmark..."
	@./script/performance.sh 127.0.0.1
	@echo ""
	@echo "╔═══════════════════════════════════════════════════════════════╗"
	@echo "║                    ✅ All tests passed!                       ║"
	@echo "╚═══════════════════════════════════════════════════════════════╝"

clean:
	rm -f $(OBJS)
	@echo "🧹 Object files cleaned"

fclean: clean
	rm -f $(NAME)
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
