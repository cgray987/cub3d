/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:17:42 by cgray             #+#    #+#             */
/*   Updated: 2024/08/05 13:42:02 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*	clears image, draws new raycasted image
	draw minimap and player
	unsure if clear_img is needed*/
static void	re_draw(t_game *game)
{
	t_point	mini_pos;
	t_point	direction;
	t_point	ray;
	double	dx;
	double	dy;

	mini_pos.x = game->player->x * game->mini_scale - 2;
	mini_pos.y = game->player->y * game->mini_scale - 2;
	dx = game->player->d->dx;
	dy = game->player->d->dy;
	direction.x = mini_pos.x + 20 * dx;
	direction.y = mini_pos.y + 20 * dy;
	ray.x = mini_pos.x + (game->mini_scale * game->perp_wall_dist) * dx;
	ray.y = mini_pos.y + (game->mini_scale * game->perp_wall_dist) * dy;
	raycast_start(game);
	draw_minimap(game);
	draw_square(game->img, mini_pos,
		5, 0x88db7bff);
	draw_line(mini_pos, direction,
		0x34c6ebff, game->img);
	draw_line(mini_pos, ray,
		0xff0000ff, game->img);
}

/*	wasd -- move up, left, right, down
	left/right -- rotate counter/clockwise
	mouse -- rotate counter/clockwise
	redraw on every hook event */
static void	key_hook(void *v_game)
{
	t_game	*game;

	game = (t_game *)v_game;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move_player(game->player, MLX_KEY_A, game->map->map);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move_player(game->player, MLX_KEY_D, game->map->map);
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move_player(game->player, MLX_KEY_W, game->map->map);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move_player(game->player, MLX_KEY_S, game->map->map);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate_player(game->player, -MOVE_SPEED);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_player(game->player, MOVE_SPEED);
	mouse_rotation(game);
	re_draw(game);
}

/*	texture flag == 0 ->game and textures has been init
				== 1 -> game has not been init
				== 2 -> game but no textures*/
static void	free_shit(t_game *game, int txt_flag)
{
	int	i;

	if (txt_flag == 2 || txt_flag == 0)
	{
		free_map_file(game->map);
		free(game->player->d);
		free(game->player->plane);
		free(game->player);
	}
	i = 0;
	while (i < 4 && txt_flag == 0)
		mlx_delete_texture(game->texture[i++]);
	free(game);
}

/*	start mlx, create image and draw first raycast proj */
static void	my_mlx_init(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", false);
	if (!game->mlx)
	{
		dprintf(2, "MLX Error!\n");
		free_shit(game, 0);
		exit(EXIT_FAILURE);
	}
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
	game->img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	// raycast_start(game);
	// draw_minimap(game);
}

int	main(int ac, char **av)
{
	t_game	*game;
	int		ret;

	if (ac == 2)
	{
		ret = 0;
		game = malloc(sizeof(t_game));
		ret = parse_data(av[1], game);
		if (ret)
		{
			free_shit(game, ret);
			exit(EXIT_FAILURE);
		}
		my_mlx_init(game);
		mlx_image_to_window(game->mlx, game->img, 0, 0);
		mlx_key_hook(game->mlx, &open_door, game);
		mlx_loop_hook(game->mlx, &key_hook, game);
		mlx_loop(game->mlx);
		mlx_terminate(game->mlx);
		free_shit(game, 0);
	}
	return (EXIT_SUCCESS);
}
