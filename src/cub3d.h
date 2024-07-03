/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:38:44 by cgray             #+#    #+#             */
/*   Updated: 2024/07/03 20:20:26 by cgray            ###   ########.fr       */
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
# define MINI_SCALE 20
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
	int		floor_color[3];
	int		ceiling_color[3];
}				t_map_file;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir;
}				t_player;


typedef struct	s_game
{
	t_player	player;
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_map_file	*map;
	uint32_t	textures[4][256][256]; // r,g,b,a, [texture height][texutre width]
	uint32_t	ceiling;
	uint32_t	floor;
}				t_game;

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
void	safe_pixel_put(mlx_image_t *img, int x, int y, uint32_t color);
void	draw_minimap(t_game *game);
void	draw_square(mlx_image_t *img, int x, int y, int size, uint32_t color);
/* ~~~~~~~~~~~~~~~~~move_player.c~~~~~~~~~~~~~~~~~~~~ */
void	check_walls(t_player *player, double new_x, double new_y, char **map);
void	rotate_player(t_player *player, double angle);
void	move_player(t_player *player, int key, char **map);

#endif
