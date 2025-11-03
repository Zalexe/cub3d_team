/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intherna <intherna@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 20:31:03 by intherna          #+#    #+#             */
/*   Updated: 2025/10/23 21:44:02 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Choose side of the cell that has the lower required
 * distance to reach the next cell boundary.
 */
void	choose_side(t_ray *ray, t_point *d, t_point *s)
{
	t_point	sd;

	sd.x = abs_d(d->x / abs_d(ray->dir.x));
	sd.y = abs_d(d->y / abs_d(ray->dir.y));
	ray->vertical = sd.x < sd.y;
	if (sd.x < sd.y)
	{
		ray->wall.x += d->x;
		ray->wall.y += d->x * (ray->dir.y / ray->dir.x);
		ray->pos.x += s->x;
		ray->pos.y += trunc(ray->wall.y) - ray->pos.y;
	}
	else
	{
		ray->wall.y += d->y;
		ray->wall.x += d->y * (ray->dir.x / ray->dir.y);
		ray->pos.y += s->y;
		ray->pos.x += trunc(ray->wall.x) - ray->pos.x;
	}
}

/**
 * Post-raycast wall boundary finding.
 */
void	calc_wall_boundaries(t_ray *ray, t_point *signs)
{
	if (ray->vertical)
	{
		ray->wall_end = (t_point){ray->wall.x, ray->pos.y + (signs->x > 0)};
		ray->wall_base = (t_point){ray->wall.x, ray->pos.y + (signs->x < 0)};
	}
	else
	{
		ray->wall_end = (t_point){ray->pos.x + (signs->y < 0), ray->wall.y};
		ray->wall_base = (t_point){ray->pos.x + (signs->y > 0), ray->wall.y};
	}
}
