


#include "cub3d.h"

//https://lodev.org/cgtutor/raycasting.html#The_Basic_Idea_
void    raycast_start(t_game *game)
{
    double  cam_x;
    double  ray_dir_x;
    double  ray_dir_y;
    int     x = 0;
    double  delta_X;
    double  delta_Y;
    double  side_dist_x;
    double  side_dist_y;
    double  perp_wall_dist;
    int     map_x_pos;
    int     map_y_pos;
    int     step_X;
    int     step_Y;
    int     hit = 0;
    int     side;

    bool    done = false;

    while (!done)
    {
        while (x < game->mlx->width - 1)
        {
            /*ray pos and direction*/
            cam_x = 2 * game->player.x / (double)game->mlx->width - 1;
            ray_dir_x = cos(game->player.dir) + 0/*PLANE X*/ * cam_x;
            ray_dir_y = sin(game->player.dir) + 0.66/*PLANE Y*/ * cam_x;
            map_x_pos = (int)game->map->map[(int)game->player.x)]; //not right
            map_y_pos = (int)game->map->map[(int)game->player.y)]; //not right

            /*len of ray from x or y to next x or y*/
            if (ray_dir_x == 0)
                delta_X = 1e30; // Infinity?
            else   
                delta_X = abs(1 / ray_dir_x);
            if (ray_dir_y == 0)
                delta_Y = 1e30; //infinity?
            else
                delta_Y = abs(1 / ray_dir_y);
            
            /*horizontal*/
            if (ray_dir_x < 0)
            {
                step_X = -1;
                side_dist_x = (game->player.x - map_x_pos)  * delta_X;
            }
            else
            {
                step_X = 1;
                side_dist_x = (map_x_pos + 1 - game->player.x) * delta_X;
            }
            /*vertical*/
            if (ray_dir_y < 0)
            {
                step_Y = -1;
                side_dist_y = (game->player.y - map_y_pos) * delta_Y;
            }
            else
            {
                step_Y = 1;
                side_dist_y = (map_y_pos + 1 - game->player.y) * delta_Y;
            }
            /*DDA*/
            while (hit == 0)
            {
                if (side_dist_x < side_dist_y)
                {
                    side_dist_x += delta_X;
                    map_x_pos += step_X;
                    side = 0;
                }
                else
                {
                    side_dist_y += delta_Y;
                    map_y_pos += step_Y;
                    side = 1;
                }
                if (game->map->map[map_y_pos][map_x_pos] == '1')
                    hit = 1;
            }
            /*proj distance*/
            if (side == 0)
                perp_wall_dist = (side_dist_x - delta_X);
            else
                perp_wall_dist = (side_dist_y - delta_Y);
            
            /*line height*/
            int line_height = (int)game->mlx->height / perp_wall_dist;
            int draw_start = -line_height / 2 + game->mlx->height / 2; //limits handled in safe_pixel
            int draw_end = line_height / 2 + game->mlx->height / 2;

            //eventually will pick color from texture
            int color1 = 0xf277a8ff;
            // int color2 = 0x67b6e0ff;
            if (side == 1)
                color1 /= 2;
            
            //draw vertical stripe
            draw_line(x, draw_start, x, draw_end, game->img); //will need to change to add color

            if (x == 0 || x == game->mlx->width - 1 || x == game->mlx->width / 2) // draw first,middle,last rays on minimap
                draw_line(game->player.x, game->player.y, side_dist_x, side_dist_y, game->img);
            x++;
        }
        done = true;
    }
}