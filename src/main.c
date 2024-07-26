/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:17:42 by cgray             #+#    #+#             */
/*   Updated: 2024/07/26 18:16:49 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//XOR swap!!!
void	swap(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

void	re_draw(t_game *game)
{
	double	mini_pos_x;
	double	mini_pos_y;
	double	dx;
	double	dy;

	clear_img(game->img);
	mini_pos_x = game->player->x * MINI_SCALE - 2;
	mini_pos_y = game->player->y * MINI_SCALE - 2;
	dx = game->player->d->dx;
	dy = game->player->d->dy;
	raycast_start(game);
	draw_minimap(game);
	draw_square(game->img, mini_pos_x, mini_pos_y,
				5, 0x88db7bff);
	draw_line(mini_pos_x, mini_pos_y,
			mini_pos_x + 20 * dx, mini_pos_y + 20 * dy, 0x34c6ebff,
			game->img); // draw direction vector on minimap
	draw_line(mini_pos_x, mini_pos_y,
			mini_pos_x + (MINI_SCALE * game->perp_wall_dist) * dx,
			mini_pos_y + (MINI_SCALE * game->perp_wall_dist) * dy,
			0xff0000ff, game->img); //draw raycast from player on minimap
}

void	mouse_rotation(t_game *game)
{
	int32_t	mouse_x;
	int32_t	mouse_y;

	mouse_x = 0;
	mouse_y = 0;
	mlx_get_mouse_pos(game->mlx, &mouse_x, &mouse_y);
	rotate_player(game->player, (mouse_x - WIDTH / 2) * 0.01);
	mlx_set_mouse_pos(game->mlx, WIDTH / 2, HEIGHT / 2);
}

static void	key_hook(void *v_game)
{
	t_game	*game;

	game = (t_game *)v_game;
	// game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move_player(game->player, MLX_KEY_A, game->map->map); // left
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move_player(game->player, MLX_KEY_D, game->map->map); // right
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move_player(game->player, MLX_KEY_W, game->map->map); // up
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move_player(game->player, MLX_KEY_S, game->map->map); // down
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate_player(game->player, -0.1); // rot counterclockwise
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_player(game->player, 0.1); // rot clockwise
	mouse_rotation(game);
	re_draw(game);
}
/* 	printf("x0:%.1f y0:%.1f x1:%.1f y1:%.1f\n", game->player.x * MINI_SCALE - 2, game->player.y * MINI_SCALE - 2,
		(game->player.x * MINI_SCALE - 2) + cos(game->player.dir) * 25,
		(game->player.y * MINI_SCALE - 2) + sin(game->player.dir) * 25); */

void	my_mlx_init(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", false);
	// errors?
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	raycast_start(game);
	draw_minimap(game);
	// errors?
}

int	main(int ac, char **av)
{
	t_game	*game;

	if (ac == 2)
	{
		game = malloc(sizeof(t_game));
		if (parse_data(av[1], game))
			exit(EXIT_FAILURE);
		my_mlx_init(game);
		mlx_image_to_window(game->mlx, game->img, 0, 0);
		mlx_loop_hook(game->mlx, &key_hook, game);
		mlx_loop(game->mlx);
		mlx_terminate(game->mlx);
		free_map_file(game->map);
		free(game->player->d);
		free(game->player->plane);
		free(game->player);
		free(game);
	}
	return (EXIT_SUCCESS);
}
