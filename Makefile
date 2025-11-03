
NAME	= cub3D
BONUS_NAME	= cub3D_bonus

CC		= cc
CFLAGS	= -Werror -Wextra -Wall -MMD -O3


MLX_PATH	= minilibx-linux/
MLX_NAME	= libmlx.a
MLX			= $(MLX_PATH)$(MLX_NAME)

LIBFT_PATH	= libft/
LIBFT_NAME	= libft.a
LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)


SRC_PATH = ./sources/
SRC		= 	main.c \
			error.c \
			exit.c \
			free_data.c \
			init/init_data.c \
			init/init_mlx.c \
			init/init_textures.c \
			parsing/check_args.c \
			parsing/parse_data.c \
			parsing/get_file_data.c \
			parsing/create_game_map.c \
			parsing/check_textures.c \
			parsing/check_map.c \
			parsing/check_map_borders.c \
			parsing/fill_color_textures.c \
			parsing/parsing_utils.c \
			movement/player_dir.c \
			movement/player_input.c \
			movement/player_move.c \
			movement/player_rotate.c \
			render/render.c \
			render/raycast.c \
			render/raycast_helpers.c \
			render/raycast_utils.c \
			render/raycast_operation.c \
			render/math_utils.c \
			render/math_utils2.c \
			render/draw.c \
			render/helpers.c

SRCS		= $(addprefix $(SRC_PATH), $(SRC))
OBJ_PATH	= ./objects/
OBJ			= $(SRC:.c=.o)
DEP			= $(SRC:.c=.d)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ))
DEPS		= $(addprefix $(OBJ_PATH), $(DEP))

BONUS_PATH = ./bonus/
BONUS_SRC	= init/init_data_bonus.c \
			  movement/player_input_bonus.c \
			  movement/player_move_bonus.c \
			  movement/player_rotate_bonus.c
BONUS_SRCS = $(filter-out \
				init/init_data.c \
				movement/player_input.c \
				movement/player_move.c \
				movement/player_rotate.c, \
				$(SRC)) \
				$(BONUS_SRC)

BONUS_OBJ_PATH = ./objects_bonus/
BONUS_OBJ	= $(BONUS_SRCS:.c=.o)
BONUS_DEP	= $(BONUS_SRCS:.c=.d)
BONUS_OBJS	= $(addprefix $(BONUS_OBJ_PATH), $(BONUS_OBJ))
BONUS_DEPS	= $(addprefix $(BONUS_OBJ_PATH), $(BONUS_DEP))


INC			=	-I ./includes/\
				-I ./libft/\
				-I ./minilibx-linux/


INC_B		=	-I ./bonus/includes/\
				-I ./libft/\
				-I ./minilibx-linux/

all: $(OBJ_PATH) $(MLX) $(LIBFT) $(NAME)

bonus: $(BONUS_OBJ_PATH) $(MLX) $(LIBFT) $(BONUS_NAME)

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)
	mkdir -p $(OBJ_PATH)/init
	mkdir -p $(OBJ_PATH)/parsing
	mkdir -p $(OBJ_PATH)/movement
	mkdir -p $(OBJ_PATH)/render

$(BONUS_OBJ_PATH):
	mkdir -p $(BONUS_OBJ_PATH)
	mkdir -p $(BONUS_OBJ_PATH)/init
	mkdir -p $(BONUS_OBJ_PATH)/parsing
	mkdir -p $(BONUS_OBJ_PATH)/movement
	mkdir -p $(BONUS_OBJ_PATH)/render

$(OBJ_PATH)%.o: $(SRC_PATH)%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(BONUS_OBJ_PATH)%.o: $(BONUS_PATH)%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_B)

$(BONUS_OBJ_PATH)%.o: $(SRC_PATH)%.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_B)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INC) $(LIBFT) $(MLX) -lXext -lX11 -lm
$(BONUS_NAME): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $@ $(LIBFT) $(MLX) -lXext -lX11 -lm

$(LIBFT):
	make -sC $(LIBFT_PATH)


$(MLX):
	make -sC $(MLX_PATH)


clean:
	rm -rf $(OBJ_PATH) $(BONUS_OBJ_PATH)
	make -C $(LIBFT_PATH) clean
	make -C $(MLX_PATH) clean


fclean: clean
	rm -f $(NAME)  $(BONUS_NAME)
	make -C $(LIBFT_PATH) fclean


re: fclean all

.PHONY: all bonus re clean fclean

-include $(DEPS)
-include $(BONUS_DEPS)
