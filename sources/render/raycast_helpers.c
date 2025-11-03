/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intherna <intherna@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:28:56 by intherna          #+#    #+#             */
/*   Updated: 2025/11/03 19:21:07 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	raycast_zero_y(t_data *d, t_ray *ray)
{
	int	delta;

	delta = (int)sign(ray->dir.y);
	while (d->map[ray->pos.y][ray->pos.x] == '0')
	{
		if (ray->pos.y < 0 || ray->pos.y >= d->mapinfo.height)
			break ;
		ray->wall.y += delta;
		ray->pos.y += delta;
	}
	ray->wall_end = (t_point){ray->pos.x + (delta < 0), ray->wall.y};
	ray->wall_base = (t_point){ray->pos.x + (delta > 0), ray->wall.y};
}

void	raycast_zero_x(t_data *d, t_ray *ray)
{
	int	delta;

	delta = (int)sign(ray->dir.x);
	while (d->map[ray->pos.y][ray->pos.x] == '0')
	{
		if (ray->pos.x < 0 || ray->pos.x >= d->mapinfo.width)
			break ;
		ray->wall.x += delta;
		ray->pos.x += delta;
	}
	ray->wall_end = (t_point){ray->wall.x, ray->pos.y + (delta > 0)};
	ray->wall_base = (t_point){ray->wall.x, ray->pos.y + (delta < 0)};
}

void	raycast_until_zero_y(t_ray *ray, t_ipoint stop)
{
	int	delta;

	delta = (int)sign(ray->dir.y);
	while (ray->pos.x != stop.x || ray->pos.y != stop.y)
	{
		ray->wall.y += delta;
		ray->pos.y += delta;
	}
	ray->wall_end = (t_point){ray->pos.x + (delta < 0), ray->wall.y};
	ray->wall_base = (t_point){ray->pos.x + (delta > 0), ray->wall.y};
}

void	raycast_until_zero_x(t_ray *ray, t_ipoint stop)
{
	int	delta;

	delta = (int)sign(ray->dir.x);
	while (ray->pos.x != stop.x || ray->pos.y != stop.y)
	{
		ray->wall.x += delta;
		ray->pos.x += delta;
	}
	ray->wall_end = (t_point){ray->wall.x, ray->pos.y + (delta > 0)};
	ray->wall_base = (t_point){ray->wall.x, ray->pos.y + (delta < 0)};
}
