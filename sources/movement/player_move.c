/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:34:19 by cmarrued          #+#    #+#             */
/*   Updated: 2025/10/16 19:12:13 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	valid_pos(t_data *data, double x, double y)
{
	double	dy;
	double	dx;
	double	margin;

	margin = 0.2;
	if (x < margin || x >= data->mapinfo.width - margin
		|| y < margin || y >= data->mapinfo.height - margin)
		return (false);
	dy = -margin;
	while (dy <= margin)
	{
		dx = -margin;
		while (dx <= margin)
		{
			if ((int)(x + dx) < 0 || (int)(x + dx) >= data->mapinfo.width
				|| (int)(y + dy) < 0 || (int)(y + dy) >= data->mapinfo.height)
				return (false);
			if (data->map[(int)(y + dy)][(int)(x + dx)] != '0')
				return (false);
			dx += 0.05;
		}
		dy += 0.05;
	}
	return (true);
}

//look if diagonal movement is possible (if needed), and if not sliding mov
static int	validate_new_pos(double new_x, double new_y, t_data *data)
{
	int	moved;

	moved = 0;
	if (valid_pos(data, new_x, new_y))
	{
		data->player.pos_x = new_x;
		data->player.pos_y = new_y;
		moved = 1;
	}
	else
	{
		if (valid_pos(data, new_x, data->player.pos_y))
		{
			data->player.pos_x = new_x;
			moved = 1;
		}
		if (valid_pos(data, data->player.pos_x, new_y))
		{
			data->player.pos_y = new_y;
			moved = 1;
		}
	}
	return (moved);
}

static int	move_forw_back(t_data *data, double dt)
{
	double	new_x;
	double	new_y;

	if (data->player.move_y == 1)
	{
		new_x = data->player.pos_x + data->player.dir_x * MOV_SPEED * dt;
		new_y = data->player.pos_y + data->player.dir_y * MOV_SPEED * dt;
	}
	if (data->player.move_y == -1)
	{
		new_x = data->player.pos_x - data->player.dir_x * MOV_SPEED * dt;
		new_y = data->player.pos_y - data->player.dir_y * MOV_SPEED * dt;
	}
	return (validate_new_pos(new_x, new_y, data));
}

static int	move_left_right(t_data *data, double dt)
{
	double	new_x;
	double	new_y;

	if (data->player.move_x == -1)
	{
		new_x = data->player.pos_x + data->player.dir_y * MOV_SPEED * dt;
		new_y = data->player.pos_y - data->player.dir_x * MOV_SPEED * dt;
	}
	if (data->player.move_x == 1)
	{
		new_x = data->player.pos_x - data->player.dir_y * MOV_SPEED * dt;
		new_y = data->player.pos_y + data->player.dir_x * MOV_SPEED * dt;
	}
	return (validate_new_pos(new_x, new_y, data));
}

int	move_direction(t_data *data, double dt)
{
	int	player_moved;

	player_moved = 0;
	if (data->player.move_y != 0)
		player_moved += move_forw_back(data, dt);
	if (data->player.move_x != 0)
		player_moved += move_left_right(data, dt);
	if (data->player.kb_rotate != 0)
		player_moved += rotate_direction(data, dt);
	return (player_moved);
}
