/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 17:32:32 by cmarrued          #+#    #+#             */
/*   Updated: 2025/10/23 21:31:21 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	get_time_in_seconds(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec + (time.tv_usec / 1000000.0));
}

// Paint ceiling and floor
static void	fill_floor_ceiling(t_data *data)
{
	int	half_y;

	half_y = data->canvas.img->height / 2;
	paint_static_color(data->texinfo.ceiling, data->canvas.addr,
		data->canvas.img->width, half_y + (data->canvas.img->height % 2));
	paint_static_color(data->texinfo.floor, get_pixel_addr_at(&data->canvas, 0,
			half_y),
		data->canvas.img->width, half_y);
}

//renderizar la vision del jugador
void	fill(t_data *data)
{
	fill_floor_ceiling(data);
	raycast(data);
}

//funcion de renderizar si el jugador se ha movido que se hookea al mlx loop
int	render(t_data *data)
{
	static double	last_time = 0;
	double			current_time;
	double			delta_time;

	current_time = get_time_in_seconds();
	if (last_time == 0)
		last_time = current_time;
	delta_time = current_time - last_time;
	data->player.has_moved += move_direction(data, delta_time);
	last_time = current_time;
	if (data->player.has_moved == 0)
		return (SUCCESS);
	data->player.has_moved = 0;
	fill(data);
	mlx_put_image_to_window(data->mlx, data->win, data->canvas.img, 0, 0);
	return (SUCCESS);
}
