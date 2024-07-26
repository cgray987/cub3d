/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:38:44 by cgray             #+#    #+#             */
/*   Updated: 2024/07/26 17:29:54 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBED_H
# define CUBED_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <math.h>
# include <stdbool.h>
# include "../lib/mlx/include/MLX42/MLX42.h"
# include "../lib/libft/includes/libft.h"

# define WIDTH 1080
# define HEIGHT 720
# define MINI_SCALE 8
# define PI 3.1415926535

typedef struct	s_cub_file
{
	bool	textures_done;
	char	*NO;
	char	*SO;
	char	*WE;
	char	*EA;
	char	*ceiling;
	char	*floor;
	char	**map;
}			t_cub_file;

typedef struct s_map_file
{
	int		map_width;
	int		map_height;
	char	**map;
	char	init_direction;
	int		init_player_x;
	int		init_player_y;
}				t_map_file;

typedef struct s_direction
{
	double	dx;
	double	dy;
}				t_direction;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir;
	t_direction	*d;
	t_direction	*plane;
}				t_player;


typedef struct	s_game
{
	t_player	*player;
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_map_file	*map;
	uint32_t	textures[4][256][256]; // r,g,b,a, [texture height][texutre width]
	uint32_t	ceiling;
	uint32_t	floor;
	double		perp_wall_dist;
}				t_game;

typedef struct s_ray
{
	double	cam_x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	delta_x;
	double	delta_y;
	int		step_x;
	int		step_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	perp_wall_dist;
	bool	hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;

}				t_ray;

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
/* ~~~~~~~~~~~~~~~~~parse_utils.c~~~~~~~~~~~~~~~~~~~~ */
void	free_array(void **array);
void	free_cub_file(t_cub_file *cub_file);
void	init_cub_file(t_cub_file *cub_file);
int		set_ceiling_floor(char *line, char *dir, t_cub_file *cub);
int		set_texture_dir(char *line, char *dir, t_cub_file *cub);
/* ~~~~~~~~~~~~~~~~~draw_mini_map.c~~~~~~~~~~~~~~~~~~~~ */
void	clear_img(mlx_image_t *img);
void	safe_pixel_put(mlx_image_t *img, int x, int y, uint32_t color);
void	draw_minimap(t_game *game);
void	draw_square(mlx_image_t *img, int x, int y, int size, uint32_t color);
/* ~~~~~~~~~~~~~~~~~move_player.c~~~~~~~~~~~~~~~~~~~~ */
void	check_collision(t_player *player, double new_x, double new_y, char **map);
void	rotate_player(t_player *player, double angle);
void	move_player(t_player *player, int key, char **map);
void	draw_line(int x0, int y0, int x1, int y1, uint32_t color, mlx_image_t *img);
/* ~~~~~~~~~~~~~~~~~color_utils.c~~~~~~~~~~~~~~~~~~~~ */
int		combine_color(int r, int g, int b);
void	color_limits(int *color_component);
int		fog(int color, double wall_dist);
/* ~~~~~~~~~~~~~~~~~raycast_utils.c~~~~~~~~~~~~~~~~~~~~ */
void	init_ray(t_ray *ray);
void	ray_direction(t_ray *ray, double x, double y);
void	ray_len(t_ray *ray, t_player *player);
void	proj_distance(t_ray *ray);

/* ~~~~~~~~~~~~~~~~~raycast.c~~~~~~~~~~~~~~~~~~~~ */
void	raycast_start(t_game *game);

#endif
