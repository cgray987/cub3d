NAME		:= cub3D
CFLAGS		:= -Wall -Werror -Wextra -g
CC 			:= cc

LIBMLX_PATH	:= ./lib/mlx
LIBMLX_NAME	:= /build/libmlx42.a
LIBMLX		:= $(LIBMLX_PATH)$(LIBMLX_NAME) -ldl -lglfw -pthread -lm -g

LIBFT_PATH	:= ./lib/libft/
LIBFT_NAME	:= libft.a
LIBFT		:= $(LIBFT_PATH)$(LIBFT_NAME) -g

INC_LIBMLX	:= -I ./include -I $(LIBMLX_PATH)/include
INC_LIBFT	:= -I ./libft

SRC_PATH	:= src/
SRC			:= color_utils.c draw_mini_map.c \
				draw.c main.c move_player.c move_utils.c \
				parse_file.c parse_free.c parse_map_init.c \
				parse_map_utils.c parse_map.c parse_utils.c \
				raycast_utils.c raycast.c textures.c
SRCS		:= $(addprefix $(SRC_PATH), $(SRC))

OBJ_PATH:= obj/
OBJ	:= ${SRC:.c=.o}
OBJS	:= $(addprefix $(OBJ_PATH), $(OBJ))

BOLD =	\033[1m
GREEN =	\033[32m
RED =	\033[31m
BCYAN =	\033[96m
NC =	\033[0m

all: libft libmlx $(NAME)

$(OBJ_PATH)%.o :$(SRC_PATH)%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC_LIBMLX) $(INC_LIBFT)

$(OBJS): | $(OBJ_PATH)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

libft:
	@echo "$(BOLD)$(BCYAN)[ Making libft... ]$(NC)"
	@make -sC $(LIBFT_PATH)
	@echo "$(BOLD)$(GREEN)[ Libft ready! ]$(NC)"

libmlx:
	@echo "$(BOLD)$(BCYAN)[ Making MLX42... ]$(NC)"
	@cmake $(LIBMLX_PATH) -B $(LIBMLX_PATH)/build && make -C $(LIBMLX_PATH)/build -j4
# -B for build -DDEBUG=1
	@echo "$(BOLD)$(GREEN)[ MLX42 ready! ]$(NC)"

$(NAME): $(OBJS)
	@echo "$(BOLD)$(BCYAN)[ Compiling $(NAME)... ]$(NC)"
	@$(CC) $(OBJS) $(LIBMLX) $(INC_LIBMLX) $(LIBFT) -o $(NAME)
	@echo "$(BOLD)$(GREEN)[ $(NAME) ready! ]$(NC)"

clean:
	@rm -Rf $(OBJ_PATH)
	@make clean -sC $(LIBFT_PATH)
	@rm -Rf $(LIBMLX_PATH)/build
	@rm -f $(LIBFT_PATH)$(LIBFT_NAME)
	@echo "$(BOLD)$(RED)[ Obj files deleted ]$(NC)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(BOLD)$(RED)[ Program deleted ]$(NC)"

re: clean all

.PHONY: all, clean, fclean, re, libmlx, libft
