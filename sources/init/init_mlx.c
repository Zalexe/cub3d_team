/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 17:23:48 by cmarrued          #+#    #+#             */
/*   Updated: 2025/10/16 19:11:36 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//reiniciamos el struct de image y guardamos una nueva imagen
void	init_img(t_data *data, t_image *image, int width, int height)
{
	clean_img(image);
	image->img = mlx_new_image(data->mlx, width, height);
	if (image->img == NULL)
		clean_exit(data, err_msg("mlx", ERR_MLX_IMG, 1));
	image->addr = (int *)mlx_get_data_addr(image->img, &image->pixel_bits,
			&image->size_line, &image->endian);
	return ;
}

//iniciamos y guardamos mlx y window en data
void	init_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		clean_exit(data, err_msg("mlx", ERR_MLX_START, 1));
	data->win = mlx_new_window(data->mlx, WIN_WIDTH, WIN_HEIGHT, "Cub3D");
	mlx_do_sync(data->mlx);
	if (!data->win)
		clean_exit(data, err_msg("mlx", ERR_MLX_WIN, 1));
	init_img(data, &data->canvas, WIN_WIDTH, WIN_HEIGHT);
}
