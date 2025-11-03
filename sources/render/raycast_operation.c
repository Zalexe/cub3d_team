/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_operation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intherna <intherna@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 19:29:09 by intherna          #+#    #+#             */
/*   Updated: 2025/10/24 18:24:17 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//printf("=== Raycast Result ===\n");
//printf("Dirs => x: %f, y: %f\n", ray->dir.x, ray->dir.y);
//printf("Base => x: %f, y: %f\n", ray->wall_base.x, ray->wall_base.y);
//printf("Hit => x: %f, y: %f\n", ray->wall.x, ray->wall.y);
//printf("End => x: %f, y: %f\n", ray->wall_end.x, ray->wall_end.y);
//printf("=== Raycast Result ===\n");
/**
 * Just raycasting, draws a straight line in the map
 * on a specified direction, and stops when it finds
 * a wall cell.
 */
void	raycast_at(char **map, t_ray *ray)
{
	t_point		deltas;
	t_point		signs;

	ray->wall = ray->from;
	signs = (t_point){sign(ray->dir.x), sign(ray->dir.y)};
	ray->pos = (t_ipoint){(int)ray->from.x, (int)ray->from.y};
	while (map[ray->pos.y][ray->pos.x] != '1')
	{
		deltas.x = (signs.x > 0) * (ceil(ray->wall.x) - ray->wall.x)
			+ (signs.x < 0) * (floor(ray->wall.x) - ray->wall.x)
			+ (signs.x * (trunc(ray->wall.x) == ray->wall.x));
		deltas.y = (signs.y > 0) * (ceil(ray->wall.y) - ray->wall.y)
			+ (signs.y < 0) * (floor(ray->wall.y) - ray->wall.y)
			+ (signs.y * (trunc(ray->wall.y) == ray->wall.y));
		choose_side(ray, &deltas, &signs);
	}
	calc_wall_boundaries(ray, &signs);
}

/**
 * Alternative raycast function, called when either direction is 0,
 * used for efficient switching and avoid constant unnecessary handling
 * of division by 0.
 * It just raycasts in a straight fixed direction without taking
 * into account the 0 side.
 */
void	raycast_at_zero(char **map, t_ray *ray)
{
	ray->pos = (t_ipoint){(int)ray->from.x, (int)ray->from.y};
	ray->wall = ray->from;
	ray->vertical = ray->dir.y == 0.0;
	if (ray->vertical)
		raycast_zero_x(map, ray);
	else
		raycast_zero_y(map, ray);
}

/**
 * Just normal raycast, but instead of stopping on a wall cell,
 * it stops at a specified point in the map.
 * Make sure the direction points to the specified cell of the map,
 * or some weird anticlimatic cataclysm may happen in the map.
 */
void	raycast_until(t_data *d, t_ray *ray, t_ipoint stop, bool vertical)
{
	t_point		deltas;
	t_point		signs;

	ray->wall = ray->from;
	signs = (t_point){sign(ray->dir.x), sign(ray->dir.y)};
	ray->pos = (t_ipoint){(int)ray->from.x, (int)ray->from.y};
	while ((vertical && ray->pos.x != stop.x)
		|| (!vertical && ray->pos.y != stop.y))
	{
		if (ray->pos.x < 0 || ray->pos.x >= d->mapinfo.width
			|| ray->pos.y < 0 || ray->pos.y >= d->mapinfo.height)
			break ;
		deltas.x = (signs.x > 0) * (ceil(ray->wall.x) - ray->wall.x)
			+ (signs.x < 0) * (floor(ray->wall.x) - ray->wall.x)
			+ (signs.x * (trunc(ray->wall.x) == ray->wall.x));
		deltas.y = (signs.y > 0) * (ceil(ray->wall.y) - ray->wall.y)
			+ (signs.y < 0) * (floor(ray->wall.y) - ray->wall.y)
			+ (signs.y * (trunc(ray->wall.y) == ray->wall.y));
		choose_side(ray, &deltas, &signs);
	}
	calc_wall_boundaries(ray, &signs);
}

/**
 * Just normal zero raycast, but instead of stopping on a wall cell,
 * it stops at a specified point in the map.
 * Make sure the direction points to the specified cell of the map,
 * or some weird anticlimatic cataclysm may happen in the map.
 */
void	raycast_until_zero(t_ray *ray, t_ipoint stop)
{
	ray->pos = (t_ipoint){(int)ray->from.x, (int)ray->from.y};
	ray->wall = ray->from;
	ray->vertical = ray->dir.y == 0.0;
	if (ray->vertical)
		raycast_until_zero_x(ray, stop);
	else
		raycast_until_zero_y(ray, stop);
}

/**
 * Raycasts at the exact last pixel of the screen,
 * called from the main raycast loop to find the
 * last wall, and render it if it was missing.
 */
void	raycast_end(t_data *d, t_loopdata *l)
{
	t_ipoint	last_pos;

	last_pos = l->ray.pos;
	l->ray.dir = get_dir_from_rads(d->player.angle + atan(1.0 * l->plane));
	if (l->ray.dir.x == 0.0 || l->ray.dir.y == 0.0)
		raycast_at_zero(d->map, &l->ray);
	else
		raycast_at(d->map, &l->ray);
	l->ray.dist = diff(l->ray.wall_base, l->ray.from);
	if (l->ray.dist <= l->last_dist)
		reverse_fix(d, l);
	l->ray.dir = get_dir(d, &l->ray.wall_base);
	if ((last_pos.x != l->ray.pos.x || last_pos.y != l->ray.pos.y)
		&& l->last_dist > l->ray.dist)
	{
		l->ray.offset_x = (((tan(l->ray.dir.rads - d->player.angle)
						/ l->plane) + 1.0) / 2.0) * WIN_WIDTH;
		l->ray.offset_x_end = round((((tan(l->ray.dir_end.rads
								- d->player.angle)
							/ l->plane) + 1.0) / 2.0) * WIN_WIDTH);
		l->ray.dir_end = get_dir(d, &l->ray.wall_end);
		fetch_texture(d, &l->ray);
		draw_wall(d, l, l->last_dist);
	}
}
