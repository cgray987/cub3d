/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:38:44 by cgray             #+#    #+#             */
/*   Updated: 2024/08/05 16:22:38 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <math.h>
# include <stdbool.h>
# include "../lib/mlx/include/MLX42/MLX42.h"
# include "../lib/libft/includes/libft.h"

# define WIDTH 1024
# define HEIGHT 768
# define MINI_SCALE 8
# define PI 3.1415926535
# define MOVE_SPEED 0.1
# define ROT_SPEED 0.23
# define FOG .1 //higher is darker

/* used to hold values while parsing */
typedef struct s_cub_file
{
	bool	textures_done;
	char	*no;
	char	*so;
	char	*we;
	char	*ea;
	char	*ceiling;
	char	*floor;
	char	**map;
}			t_cub_file;

/* held in main struct */
typedef struct s_map_file
{
	int		map_width;
	int		map_height;
	char	**map;
	char	init_direction;
	int		init_player_x;
	int		init_player_y;
	int		texture_size;
}				t_map_file;

/* norm.... */
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
	int	a;
}			t_color;

/* norm.... */
typedef struct s_point
{
	int	x;
	int	y;
}			t_point;

typedef struct s_direction
{
	double	dx;
	double	dy;
}				t_direction;

/* player information */
typedef struct s_player
{
	double		x;
	double		y;
	double		dir;	//goes from 0-2pi
	t_direction	*d;		//x and y components of direction
	t_direction	*plane;	//x and y componets of plane direction
}				t_player;

/* main game struct, contains player info, mlx pointers, textures and colors */
typedef struct s_game
{
	t_player		*player;	//location/direction
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_map_file		*map;
	int				ceiling;
	int				floor;
	mlx_texture_t	*texture[4];
	int				mini_scale;
	double			perp_wall_dist; //used for drawing on minimap
	double			door_dist;
}				t_game;

/* Variable dump for raycasting calculations, */
typedef struct s_ray
{
	int		x;				//current x coord in screen
	double	cam_x;			//x coord in camera space
	double	ray_dir_x;		//x component of ray direction
	double	ray_dir_y;		//y component of ray direction
	double	delta_x;		//x component of distance to next x side
	double	delta_y;		//y component of distance to next y side
	int		step_x;			//x component neg or positive
	int		step_y;			//ycomponent neg or positive
	int		map_x;			//x coord of map position
	int		map_y;			//y coord of map position
	double	side_dist_x;	//ray dist from start to next x, incremented by step
	double	side_dist_y;	//ray dist from start to next y, incremented by step
	double	perp_wall_dist;	//side dist perpendicular to camera plane
	bool	door_hit;
	bool	hit;			//set when ray hits wall
	int		side;			//1 == N/S, 2 == E/W
	int		line_height;	//height of vertical line drawn on screen
	int		draw_start;		//y coord of start of line on screen
	int		draw_end;		//y coord of end of line on screen
}				t_ray;

/* variable dump for texture calcs */
typedef struct s_txt_math
{
	int			txt_index;
	double		wall_x; //where wall was hit
	int			txt_x; //x coord of texture
	int			txt_width;
	double		step;
	double		txt_pos;
	int			txt_y;
	int			color;
	int			pixel_index;
}				t_txt_math;

/* deprecated? */
typedef struct s_map_valid
{
	bool	valid_direction;
	bool	valid_ceiling;
	bool	valid_floor;
	bool	valid_walls;
}				t_map_valid;

/* ~~~~~~~~~~~~~~~~~parse_file.c~~~~~~~~~~~~~~~~~~~~~~ */
int		parse_data(char *arg, t_game *game);
/* ~~~~~~~~~~~~~~~~~parse_map.c~~~~~~~~~~~~~~~~~~~~ */
bool	parse_map(t_cub_file *cub_file, t_game *game);
/* ~~~~~~~~~~~~~~~~~parse_map_utils.c~~~~~~~~~~~~~~~~~~~~ */
char	**dup_array(char **array, int height);
int		get_map_height(char **map);
int		get_map_width(char **map);
bool	init_map_file(t_map_file *map_file);
void	free_map_file(t_map_file *map_file);
void	print_fill_map(t_map_file *map_file, char **fill_map);
/* ~~~~~~~~~~~~~~~~~parse_map_init.c~~~~~~~~~~~~~~~~~~~~ */
void	get_init_direction(t_game *game);
void	init_player(t_game *game);
int		get_ceiling_floor_color(char *color);
bool	load_texture_error(t_game *game, t_cub_file *cub_file);
bool	load_textures(t_game *game, t_cub_file *cub_file);
/* ~~~~~~~~~~~~~~~~~parse_utils.c~~~~~~~~~~~~~~~~~~~~ */
void	free_array(void **array);
void	free_cub_file(t_cub_file *cub_file);
void	init_cub_file(t_cub_file *cub_file);
int		set_ceiling_floor(char *line, char *dir, t_cub_file *cub);
int		set_texture_dir(char *line, char *dir, t_cub_file *cub);
/* ~~~~~~~~~~~~~~~~~draw.c~~~~~~~~~~~~~~~~~~~~ */
void	clear_img(mlx_image_t *img);
void	safe_pixel_put(mlx_image_t *img, int x, int y,
			uint32_t color);
/* ~~~~~~~~~~~~~~~~~draw_mini_map.c~~~~~~~~~~~~~~~~~~~~ */
void	draw_minimap(t_game *game);
void	draw_square(mlx_image_t *img, t_point p, int size, uint32_t color);
void	draw_line(t_point start, t_point end, uint32_t color, mlx_image_t *img);
/* ~~~~~~~~~~~~~~~~~move_player.c~~~~~~~~~~~~~~~~~~~~ */
void	rotate_player(t_player *player, double angle);
void	move_player(t_player *player, int key, char **map);
void	mouse_rotation(t_game *game);
void	open_door(mlx_key_data_t key, void *v_game);
/* ~~~~~~~~~~~~~~~~~move_utils.c~~~~~~~~~~~~~~~~~~~~ */
void	check_collision(t_player *player, double new_x,
			double new_y, char **map);
void	door_handler(t_game *game, int i, int j);
/* ~~~~~~~~~~~~~~~~~color_utils.c~~~~~~~~~~~~~~~~~~~~ */
int		combine_color(int r, int g, int b, int a);
void	color_limits(int *color_component);
int		fog(int color, double wall_dist);
/* ~~~~~~~~~~~~~~~~~raycast_utils.c~~~~~~~~~~~~~~~~~~~~ */
void	init_ray(t_ray *ray);
void	ray_direction(t_ray *ray, double x, double y);
void	ray_len(t_ray *ray, t_player *player);
void	proj_distance(t_ray *ray, double *door_dist);
/* ~~~~~~~~~~~~~~~~~textures.c~~~~~~~~~~~~~~~~~~~~ */
void	draw_texture(int x, int y, t_game *game, t_ray *ray);
/* ~~~~~~~~~~~~~~~~~raycast.c~~~~~~~~~~~~~~~~~~~~ */
void	raycast_start(t_game *game);

#endif
