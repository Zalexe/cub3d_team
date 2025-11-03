/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intherna <intherna@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 21:39:33 by intherna          #+#    #+#             */
/*   Updated: 2025/10/23 22:16:02 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*printf("persp_factor: %f, x: %d, dist_start: %.20f,
dist_end: %.20f, factor: %f\n", persp_factor, x,
dd->dist_start, dd->dist_end, factor);*/
/**
 * Gets the perspective corrected pixel in a texture.
 */
int	get_x_scaled(t_draw_data *dd, int j, int texsize)
{
	double	factor;
	double	inv_interp;
	double	persp_factor;
	int		x;

	factor = (double)j / dd->proj_size;
	if (abs_d(dd->dist_start - dd->dist_end) < 1e-6)
		persp_factor = factor;
	else
	{
		inv_interp = 1.0 / ((1.0 - factor)
				/ dd->dist_start + factor / dd->dist_end);
		persp_factor = (inv_interp - dd->dist_start)
			/ (dd->dist_end - dd->dist_start);
	}
	x = dd->ioffset_x + ((texsize - 1 - dd->ioffset_x) * persp_factor);
	return (x);
}

/**
 * Fix in case the wall_end is hidden, raycasts at the end of the screen
 * to find the visible portion of the wall.
 */
void	fix(t_data *d, t_draw_data *ddata, t_loopdata *l)
{
	t_dir	tmp;

	tmp = get_dir_from_rads(d->player.angle + atan(1.0 * l->plane));
	if (abs_d(tmp.rads - l->ray.dir.rads) > 1e-6)
	{
		l->ray.dir = tmp;
		if (l->ray.dir.x == 0.0 || l->ray.dir.y == 0.0)
			raycast_until_zero(&l->ray, l->ray.pos);
		else
			raycast_until(d, &l->ray, l->ray.pos, l->ray.vertical);
	}
	ddata->dist_end = diff(l->ray.from, l->ray.wall)
		* cos(l->ray.dir.rads - d->player.angle);
	l->ray.offset_x_end = WIN_WIDTH;
	if (l->ray.vertical)
		ddata->img_width = abs_d(
				l->ray.wall.y - l->ray.wall_base.y) * TEXTURE_SIZE;
	else
		ddata->img_width = abs_d(
				l->ray.wall.x - l->ray.wall_base.x) * TEXTURE_SIZE;
}

//printf("Angle diff: %f, fov: %f\n",
//angle_diff(d->player.angle, l->ray.dir_end.rads), FOV / 2.0);
//printf("offsetx: %d, offsetx_end: %d, ld: %f, dist_start: %f,
//dist_end: %f, start: %d, end: %d,proj_size: %d\n", l->ray.offset_x,
//l->ray.offset_x_end,dd->factor, dd->dist_start, dd->dist_end,
//dd->start,dd->end, dd->proj_size);
/**
 * Set the boundaries for the wall to be drawn.
 */
void	set_boundaries(t_data *d, t_draw_data *dd, t_loopdata *l, double ld)
{
	t_point	*wall;

	wall = &l->ray.wall_base;
	if (l->ray.dist >= ld)
		wall = &l->ray.wall;
	if (l->ray.vertical)
		dd->ioffset_x = (TEXTURE_SIZE - 1)
			* (1.0 - abs_d(wall->y - l->ray.wall_end.y));
	else
		dd->ioffset_x = (TEXTURE_SIZE - 1)
			* (1.0 - abs_d(wall->x - l->ray.wall_end.x));
	dd->factor = (WIN_WIDTH / 2.0) / tan(FOV / 2);
	dd->dist_start = l->ray.dist * cos(l->ray.dir.rads - d->player.angle);
	dd->start = (int)round(dd->factor / dd->dist_start);
	dd->img_width = TEXTURE_SIZE;
	if (angle_diff(d->player.angle, l->ray.dir_end.rads) >= (FOV / 2.0))
		fix(d, dd, l);
	else
		dd->dist_end = diff(l->ray.from, l->ray.wall_end)
			* cos(l->ray.dir_end.rads - d->player.angle);
	dd->end = (int)round(dd->factor / dd->dist_end);
	dd->proj_size = max_i(abs_i(l->ray.offset_x_end - l->ray.offset_x), 1);
	dd->height_max = max_i(dd->start, dd->end);
	dd->diff = dd->end - dd->start;
	dd->j = 0;
}

/*printf("col_size: %d, soffset_y: %d, factor: %f,
i: %d, y: %d, ioffset_x: %d, img_width: %d, j: %d\n",
dd.column_size, dd.soffset_y, dd.factor, dd.i, dd.y,
dd.ioffset_x, dd.img_width, dd.j);*/
/**
 * It draws a wall in the screen using interpolation.
 */
int	draw_wall(t_data *d, t_loopdata *l, double ld)
{
	t_draw_data	dd;

	set_boundaries(d, &dd, l, ld);
	while (dd.j <= min_i(dd.proj_size, WIN_WIDTH - l->ray.offset_x - 1))
	{
		dd.column_size = dd.start + (dd.diff * ((double)(dd.j) / dd.proj_size));
		dd.soffset_y = max_i((d->canvas.img->height - dd.column_size) / 2, 0);
		dd.i = 0;
		dd.y = max_i(0, (dd.column_size - WIN_HEIGHT) / 2);
		while (dd.i < min_i(dd.column_size, WIN_HEIGHT))
		{
			dd.addr = get_pixel_addr_at(&d->canvas,
					l->ray.offset_x + dd.j, dd.i + dd.soffset_y);
			dd.factor_y = (double)dd.y / dd.column_size;
			*dd.addr = *(int32_t *)iget_pixel_addr_at(l->ray.txt,
					l->ray.txtdata, get_x_scaled(&dd, dd.j, dd.img_width),
					(TEXTURE_SIZE - 1) * dd.factor_y);
			dd.i++;
			dd.y++;
		}
		dd.j++;
	}
	return (dd.proj_size);
}
