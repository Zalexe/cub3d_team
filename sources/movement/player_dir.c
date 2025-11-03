/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 17:30:51 by cmarrued          #+#    #+#             */
/*   Updated: 2025/10/06 18:47:14 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	player_north_south(t_player *player)
{
	if (player->dir == 'S')
	{
		player->dir_x = 0;
		player->dir_y = 1;
		player->fov_x = -FOV;
		player->fov_y = 0;
	}
	else if (player->dir == 'N')
	{
		player->dir_x = 0;
		player->dir_y = -1;
		player->fov_x = FOV;
		player->fov_y = 0;
	}
	else
		return ;
}

static void	player_east_west(t_player *player)
{
	if (player->dir == 'W')
	{
		player->dir_x = -1;
		player->dir_y = 0;
		player->fov_x = 0;
		player->fov_y = -FOV;
	}
	else if (player->dir == 'E')
	{
		player->dir_x = 1;
		player->dir_y = 0;
		player->fov_x = 0;
		player->fov_y = FOV;
	}
	else
		return ;
}

void	init_player_direction(t_data *data)
{
	player_north_south(&data->player);
	player_east_west(&data->player);
	data->player.angle = atan2(data->player.dir_y, data->player.dir_x);
}
