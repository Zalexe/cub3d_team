/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:36:08 by cmarrued          #+#    #+#             */
/*   Updated: 2025/10/25 20:46:16 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "mlx.h"

void	clean_exit(t_data *data, int code)
{
	if (!data)
		exit(code);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		if (data->textu_img[0].img)
			mlx_destroy_image(data->mlx, data->textu_img[0].img);
		if (data->textu_img[1].img)
			mlx_destroy_image(data->mlx, data->textu_img[1].img);
		if (data->textu_img[2].img)
			mlx_destroy_image(data->mlx, data->textu_img[2].img);
		if (data->textu_img[3].img)
			mlx_destroy_image(data->mlx, data->textu_img[3].img);
		mlx_destroy_display(data->mlx);
		mlx_loop_end(data->mlx);
		free(data->mlx);
	}
	free_data(data);
	exit(code);
}

int	quit_cub3d(t_data *data)
{
	clean_exit(data, 0);
	return (SUCCESS);
}
