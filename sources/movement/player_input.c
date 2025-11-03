/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:34:19 by cmarrued          #+#    #+#             */
/*   Updated: 2025/10/16 19:05:22 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	press_handler(int key, t_data *data)
{
	if (key == XK_Escape)
		quit_cub3d(data);
	if (key == XK_w)
		data->player.move_y = 1;
	if (key == XK_s)
		data->player.move_y = -1;
	if (key == XK_d)
		data->player.move_x = 1;
	if (key == XK_a)
		data->player.move_x = -1;
	if (key == XK_Left)
		data->player.kb_rotate -= 1;
	if (key == XK_Right)
		data->player.kb_rotate += 1;
	return (SUCCESS);
}

static int	release_handler(int key, t_data *data)
{
	if (key == XK_Escape)
		quit_cub3d(data);
	if (key == XK_w && data->player.move_y == 1)
		data->player.move_y = 0;
	if (key == XK_s && data->player.move_y == -1)
		data->player.move_y = 0;
	if (key == XK_d && data->player.move_x == 1)
		data->player.move_x = 0;
	if (key == XK_a && data->player.move_x == -1)
		data->player.move_x = 0;
	if (key == XK_Left && data->player.kb_rotate <= 1)
		data->player.kb_rotate = 0;
	if (key == XK_Right && data->player.kb_rotate >= -1)
		data->player.kb_rotate = 0;
	return (SUCCESS);
}

void	listen_input(t_data *data)
{
	mlx_hook(data->win, ClientMessage, NoEventMask, quit_cub3d, data);
	mlx_hook(data->win, KeyPress, KeyPressMask, press_handler, data);
	mlx_hook(data->win, KeyRelease, KeyReleaseMask, release_handler, data);
}
