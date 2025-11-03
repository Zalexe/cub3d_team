/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_rotate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:34:19 by cmarrued          #+#    #+#             */
/*   Updated: 2025/10/06 18:49:47 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	rotate(t_data *data, double total_rot)
{
	double	old_dir_x;
	double	old_fov_x;
	double	rot;

	rot = total_rot;
	old_dir_x = data->player.dir_x;
	old_fov_x = data->player.fov_x;
	data->player.dir_x = data->player.dir_x * cos(rot)
		- data->player.dir_y * sin(rot);
	data->player.dir_y = old_dir_x * sin(rot)
		+ data->player.dir_y * cos(rot);
	data->player.fov_x = data->player.fov_x * cos(rot)
		- data->player.fov_y * sin(rot);
	data->player.fov_y = old_fov_x * sin(rot)
		+ data->player.fov_y * cos(rot);
	return (1);
}

int	rotate_direction(t_data *data, double dt)
{
	double	total_rotate;
	int		player_moved;

	total_rotate = data->player.kb_rotate * ROT_SPEED * dt;
	player_moved = 0;
	if (total_rotate != 0)
	{
		player_moved += rotate(data, total_rotate);
		data->player.angle = atan2(data->player.dir_y, data->player.dir_x);
	}
	return (player_moved);
}
