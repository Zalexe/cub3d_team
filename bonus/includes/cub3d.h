/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:32:52 by cmarrued          #+#    #+#             */
/*   Updated: 2025/10/23 20:09:04 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "mlx.h"
# include "mlx_int.h"
# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <unistd.h>

/* -----------------------  DEFINES  ---------------------------------- */

# define WIN_WIDTH 640
# define WIN_HEIGHT 480

# define FOV 1.15

# ifndef O_DIRECTORY
#  define O_DIRECTORY 00200000 /* Byte of the flag for directories*/
# endif

# define TEXTURE_SIZE 64

//movement
# define ROT_SPEED 2
# define MOV_SPEED 2
# define MAX_SPEED_STEP 0.1
// ERROR MESSAGES
# define ERR_USAGE "usage: ./cub3d <path/to/map.cub>"
# define ERR_MALLOC "Could not allocate memory"

//File errors
# define ERR_FILE_NOT_CUB "Not a .cub file"
# define ERR_FILE_NOT_XPM "Not an .xpm file"
# define ERR_FILE_IS_DIR "Is a directory"

//Color errors
# define ERR_FLOOR_CEILING "Invalid floor/ceiling RGB color(s)"
# define ERR_COLOR_FLOOR "Invalid floor RGB color"
# define ERR_COLOR_CEILING "Invalid ceiling RGB color"

//Map header errors
# define ERR_TEX_RGB_VAL "Invalid RGB value (min: 0, max: 255)"
# define ERR_TEX_MISSING "Missing texture(s)"
# define ERR_TEX_INVALID "Invalid texture(s)"
# define ERR_COLOR_MISSING "Missing color(s)"

//Map errors
# define ERR_MAP_MISSING "Missing map"
# define ERR_INVALID_MAP "Map description is either wrong or incomplete"
# define ERR_INV_LETTER "Invalid character in map"
# define ERR_NUM_PLAYER "Map has more than one player"
# define ERR_MAP_TOO_SMALL "Map is not at least 3 lines high"
# define ERR_MAP_NO_WALLS "Map is not surrounded by walls"
# define ERR_MAP_LAST "Map is not the last element in file"

//Player errors
# define ERR_PLAYER_POS "Invalid player position"
# define ERR_PLAYER_DIR "Map has no player position (expected N, S, E or W)"

//MLX error
# define ERR_MLX_START "Could not start mlx"
# define ERR_MLX_WIN "Could not create mlx window"
# define ERR_MLX_IMG "Could not create mlx image"

/* outputs exit*/
enum e_output
{
	SUCCESS = 0,
	FAILURE = 1,
	ERR = 2,
	BREAK = 3,
	CONTINUE = 4
};

typedef unsigned long	t_ulong;

/* -----------------------  STRUCTS  ------------------------------------ */

typedef struct s_dir
{
	double	x;
	double	y;
	double	rads;
}	t_dir;

typedef struct s_point
{
	double	x;
	double	y;
}	t_point;

typedef struct s_ipoint
{
	int	x;
	int	y;
}	t_ipoint;

/* struct data image*/
typedef struct s_image
{
	t_img		*img; /*pointer to MLX img object*/
	int			*addr; /*pointer to the raw pixel data*/
	int			pixel_bits; /* bits per pizel*/
	int			size_line; /*bytes per row of the image */
	int			endian; /*endian pixel data*/
}	t_image;

/* Struct for textures */
typedef struct s_texinfo
{
	char			*north; /* path texture north */
	char			*south; /* path texture south */
	char			*west; /* path texture west */
	char			*east; /* path texture east */
	int32_t			floor; /* color int*/
	int32_t			ceiling; /* color ceiling */
	int				size; /*size textures */
}	t_texinfo;

/*struct info of map*/
typedef struct s_mapinfo
{
	int			fd; /*fd of map file*/
	int			line_count; /* num of lines*/
	char		*path; /* path to file */
	char		**file; /* file content */
	int			height; /*height of map */
	int			width; /* width of map */
	int			index_end_of_map; /* index to end of file map */
}	t_mapinfo;

typedef struct s_ray
{
	t_dir		dir; /* dir of ray */
	t_dir		dir_end;
	t_point		from;
	t_ipoint	pos;
	t_point		wall_base;
	t_point		wall;
	t_point		wall_end;
	bool		vertical;
	double		dist;
	int			offset_x;
	int			offset_x_end;
	uint32_t	*txt; /* Texture */
	t_image		*txtdata;
}	t_ray;

typedef struct s_loopdata
{
	int			x;
	double		cam_x;
	double		plane;
	t_ray		ray;
	double		last_dist;
	t_ipoint	last_pos;
}	t_loopdata;

typedef struct s_draw_data
{
	double		factor;
	double		factor_y;
	int			start;
	int			end;
	double		dist_start;
	double		dist_end;
	int			proj_size;
	int			height_max;
	int			column_size;
	int			soffset_y;
	int			ioffset_x;
	int			img_width;
	int			i;
	int			j;
	int			y;
	int			diff;
	uint32_t	*addr;
}	t_draw_data;

/* Data for player */
typedef struct s_player
{
	char	dir; /* Direction is looking */
	double	pos_x; /* position in X */
	double	pos_y; /* Position in Y */
	double	dir_x; /* direction in X */
	double	dir_y; /* Direction in Y */
	double	angle; /* Angle direction in radians */
	double	fov_x; /* camera plane vector X (FOV)*/
	double	fov_y;
	int		has_moved; /* Has moved Flag */
	int		move_x; /* moved in X */
	int		move_y; /* moved in Y */
	double	kb_rotate; /* key rotate */
	double	mouse_rotate; /* mouse rotate */
	int		last_mouse_x; /* last position of mouse in X */
}	t_player;

/* Data of the program */
typedef struct s_data
{
	void			*mlx; /* pointer to MLX */
	void			*win; /* Pointer to window */
	int				win_height; /* height window */
	int				win_width; /* width window */
	t_mapinfo		mapinfo; /* struct with map info */
	char			**map; /* file map */
	t_player		player; /* struct player data*/
	int				**textures; /* array con las texturas */
	t_texinfo		texinfo; /* data textures */
	t_image			textu_img[4];
	t_image			canvas; /* Reusable screen frame */
	struct timeval	last_frame_time; /* time last frame */
	double			delta_time; /* current time program */
}	t_data;

/* -----------------------  FUNCTIONS  ------------------------------------ */

/* init/init_data.c */
void	init_data(t_data *data);
void	clean_img(t_image *img);

/* init/init_mlx.c */
void	init_mlx(t_data *data);
void	init_img(t_data *data, t_image *image, int width, int height);

/* init/init_textures.c */
void	init_textures(t_data *data);
void	init_texstruct(t_texinfo *textures);
void	init_texture_img(t_data *data, t_image *image, char *path);

/* parsing/check_args.c */
int		check_file(char *arg, bool cub);

/* parsing/parse_data.c */
void	parse_data(char *path, t_data *data);

/* parsing/get_file_data.c */
int		get_file_data(t_data *data, char **map);

/* parsing/fill_color_textures.c */
int		fill_color_textures(t_data *data, t_texinfo *textures,
			char *line, int j);

/* parsing/create_game_map.c */
int		create_map(t_data *data, char **map, int i);

/* parsing/check_textures.c */
int		check_textures_validity(t_data *data, t_texinfo *textures);

/* parsing/check_map.c */
int		check_map_validity(t_data *data, char **map_tab);

/* parsing/check_map_borders.c */
int		check_map_sides(t_mapinfo *map, char **map_tab);

/* parsing/parsing_utils.c */
int		is_white_space(char c);
size_t	find_biggest_len(t_mapinfo *map, int i);

/* render/render.c */
int		render(t_data *data);
void	fill(t_data *data);

/* render/raycast.c */
void	raycast(t_data *data);
void	raycast_at(char **map, t_ray *ray);
void	raycast_at_zero(char **map, t_ray *ray);
void	raycast_end(t_data *d, t_loopdata *ldata);
void	raycast_until_zero(t_ray *ray, t_ipoint stop);
void	raycast_until(t_data *d, t_ray *ray, t_ipoint stop, bool vertical);
void	raycast_zero_y(char **map, t_ray *ray);
void	raycast_zero_x(char **map, t_ray *ray);
void	raycast_until_zero_y(t_ray *ray, t_ipoint stop);
void	raycast_until_zero_x(t_ray *ray, t_ipoint stop);
void	fetch_texture(t_data *data, t_ray *ray);
void	reverse_fix(t_data *data, t_loopdata *d);
void	choose_side(t_ray *ray, t_point *d, t_point *s);
void	calc_wall_boundaries(t_ray *ray, t_point *signs);

/* render/helpers.c */
void	*get_pixel_addr_at(t_image *img, uint32_t x, uint32_t y);
void	*iget_pixel_addr_at(uint32_t *img, t_image *in, uint32_t x, uint32_t y);
void	paint_static_color(uint32_t color, void *p, int x, int y);
t_dir	get_dir_from_rads(double rads);
t_dir	get_dir(t_data *data, t_point *p);

/* render/draw.c */
int		draw_wall(t_data *data, t_loopdata *l, double ld);

/* render/math_utils.c */
double	max_d(double d1, double d2);
double	min_d(double d1, double d2);
int		max_i(int i1, int i2);
int		min_i(int i1, int i2);
double	abs_d(double d);
int		abs_i(int i);
double	sign(double d);
int		sign_i(int i);
double	diff(t_point p1, t_point p2);
double	angle_diff(double rads1, double rads2);

/* movement/player_dir.c */
void	init_player_direction(t_data *data);
/* movement/player_input.c */
void	listen_input(t_data *data);
/* movement/player_move.c */
int		move_direction(t_data *data, double dt);
/* movement/player_rotate.c */
int		rotate_direction(t_data *data, double dt);

/* error.c */
int		err_msg(char *detail, char *str, int code);
int		err_msg_val(int detail, char *str, int code);

/* exit.c */
void	clean_exit(t_data *data, int code);
int		quit_cub3d(t_data *data);

/* free_data.c */
void	free_tab(void **tab);
int		free_data(t_data *data);

#endif
