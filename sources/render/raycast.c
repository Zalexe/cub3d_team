/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intherna <intherna@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 18:00:43 by intherna          #+#    #+#             */
/*   Updated: 2025/10/29 16:45:25 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Fetch correct texture based on the direction of the wall
 * and the side it hit.
 */
void	fetch_texture(t_data *data, t_ray *ray)
{
	if (ray->dir.y > 0)
	{
		ray->txt = (uint32_t *)data->textures[1];
		ray->txtdata = &data->textu_img[1];
	}
	else
	{
		ray->txt = (uint32_t *)data->textures[0];
		ray->txtdata = &data->textu_img[0];
	}
	if (ray->vertical)
	{
		if (ray->dir.x > 0)
		{
			ray->txt = (uint32_t *)data->textures[2];
			ray->txtdata = &data->textu_img[2];
		}
		else
		{
			ray->txt = (uint32_t *)data->textures[3];
			ray->txtdata = &data->textu_img[3];
		}
	}
}

/**
 * Raycast and initialize some necessary data.
 */
void	prepare(t_data *data, t_ray *ray, double ld)
{
	ray->from = (t_point){data->player.pos_x, data->player.pos_y};
	if (ray->dir.x == 0.0 || ray->dir.y == 0.0)
		raycast_at_zero(data->map, ray);
	else
		raycast_at(data->map, ray);
	ray->dir_end = get_dir(data, &ray->wall_end);
	ray->dist = diff(ray->wall, ray->from);
	if (ray->dist < ld)
		ray->dist = diff(ray->wall_base, ray->from);
	fetch_texture(data, ray);
}

/**
 * Reverse loop to find missed walls.
 */
static void	loop_reverse(t_data *data, t_loopdata *cl, t_loopdata *d)
{
	while ((cl->ray.offset_x > d->ray.offset_x
			|| (cl->ray.offset_x == d->ray.offset_x
				&& cl->ray.dist < d->last_dist))
		&& cl->ray.offset_x > 0 && cl->ray.offset_x < WIN_WIDTH)
	{
		cl->ray.offset_x_end = round((((tan(cl->ray.dir_end.rads
								- data->player.angle) / d->plane)
						+ 1.0) / 2.0) * WIN_WIDTH);
		draw_wall(data, cl, d->last_dist);
		cl->x = cl->ray.offset_x - 2;
		cl->cam_x = 2.0 * cl->x / (double)WIN_WIDTH - 1.0;
		cl->ray.dir = get_dir_from_rads(data->player.angle
				+ atan(cl->cam_x * d->plane));
		if (cl->ray.dir.x == 0.0 || cl->ray.dir.y == 0.0)
			raycast_at_zero(data->map, &cl->ray);
		else
			raycast_at(data->map, &cl->ray);
		cl->ray.dir_end = get_dir(data, &cl->ray.wall_end);
		cl->ray.dist = diff(cl->ray.wall_base, cl->ray.from);
		fetch_texture(data, &cl->ray);
		cl->ray.dir = get_dir(data, &cl->ray.wall_base);
		cl->ray.offset_x = (((tan(cl->ray.dir.rads - data->player.angle)
						/ d->plane) + 1.0) / 2.0) * WIN_WIDTH;
	}
}

/**
 * Go back to the start boundary of the previously found
 * wall, then try to iterate backwards to find missed walls.
 */
void	reverse_fix(t_data *data, t_loopdata *d)
{
	t_loopdata	cl;

	d->ray.dir = get_dir(data, &d->ray.wall_base);
	d->cam_x = tan(d->ray.dir.rads - data->player.angle) / d->plane;
	d->x = ((d->cam_x + 1.0) / 2.0) * WIN_WIDTH;
	cl.x = d->x - 1;
	cl.cam_x = 2.0 * cl.x / (double)WIN_WIDTH - 1.0;
	cl.ray.dir = get_dir_from_rads(data->player.angle
			+ atan(cl.cam_x * d->plane));
	cl.ray.from = (t_point){data->player.pos_x, data->player.pos_y};
	if (cl.ray.dir.x == 0.0 || cl.ray.dir.y == 0.0)
		raycast_at_zero(data->map, &cl.ray);
	else
		raycast_at(data->map, &cl.ray);
	cl.ray.dir_end = cl.ray.dir;
	cl.ray.dist = diff(cl.ray.wall_base, cl.ray.from);
	fetch_texture(data, &cl.ray);
	cl.ray.dir = get_dir(data, &cl.ray.wall_base);
	if (angle_diff(data->player.angle, cl.ray.dir.rads) > (FOV / 2))
		return ;
	cl.ray.offset_x = (((tan(cl.ray.dir.rads - data->player.angle)
					/ d->plane) + 1.0) / 2.0) * WIN_WIDTH;
	loop_reverse(data, &cl, d);
}

/**
 * Raycasting loop.
 *
 * Procedure: Wall-based raycasting
 * Starts the loop from the left of the screen,
 * Renders a wall, and then jumps to the direction
 * where the next wall should be (may not be there,
 * but its only to get the direction), and then raycasts there,
 * and so on until the end of the screen, making it by far
 * more efficient than traditional raycasting.
 *
 * If a wall is closer than the last wall, it will go back
 * some pixels to account the missed part of the wall, and
 * will iterate backwards to try to find missed walls.
 */
void	raycast(t_data *data)
{
	t_loopdata	d;

	d.last_dist = -1.0;
	d.plane = tan(FOV / 2);
	d.x = 0;
	while (d.x < WIN_WIDTH && d.x >= 0)
	{
		d.cam_x = (2.0 * d.x / (double)WIN_WIDTH) - 1.0;
		d.ray.dir = get_dir_from_rads(data->player.angle
				+ atan(d.cam_x * d.plane));
		prepare(data, &d.ray, d.last_dist);
		if (d.ray.dist < d.last_dist)
			reverse_fix(data, &d);
		d.ray.offset_x = d.x;
		d.ray.offset_x_end = round((((tan(d.ray.dir_end.rads
								- data->player.angle)
							/ d.plane) + 1.0) / 2.0) * WIN_WIDTH);
		d.x += draw_wall(data, &d, d.last_dist) + 1;
		d.last_dist = d.ray.dist;
		if (d.ray.offset_x > d.ray.offset_x_end)
			break ;
	}
	if ((d.ray.offset_x_end - d.ray.offset_x) < WIN_WIDTH)
		raycast_end(data, &d);
}
