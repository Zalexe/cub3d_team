/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 17:24:24 by cmarrued          #+#    #+#             */
/*   Updated: 2025/10/01 15:47:16 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//reiniciamos el struct de imagen y guardamos una nueva textura
void	init_texture_img(t_data *data, t_image *image, char *path)
{
	clean_img(image);
	image->img = mlx_xpm_file_to_image(data->mlx, path, &data->texinfo.size,
			&data->texinfo.size);
	if (image->img == NULL)
		clean_exit(data, err_msg("mlx", ERR_MLX_IMG, 1));
	image->addr = (int *)mlx_get_data_addr(image->img, &image->pixel_bits,
			&image->size_line, &image->endian);
	return ;
}

//convertimos la textura en imagen y la devolvemos
static int	*xpm_to_img(t_data *data, char *path)
{
	t_image	tmp;
	int		*buffer;
	int		x;
	int		y;

	init_texture_img(data, &tmp, path);
	buffer = ft_calloc(1,
			sizeof * buffer * data->texinfo.size * data->texinfo.size);
	if (!buffer)
		clean_exit(data, err_msg(NULL, ERR_MALLOC, 1));
	y = 0;
	while (y < data->texinfo.size)
	{
		x = 0;
		while (x < data->texinfo.size)
		{
			buffer[y * data->texinfo.size + x]
				= tmp.addr[y * data->texinfo.size + x];
			++x;
		}
		y++;
	}
	mlx_destroy_image(data->mlx, tmp.img);
	return (buffer);
}

//guardamos en data las imagenes de las texturas
void	init_textures(t_data *data)
{
	data->textures = ft_calloc(5, sizeof * data->textures);
	if (!data->textures)
		clean_exit(data, err_msg(NULL, ERR_MALLOC, 1));
	init_texture_img(data, &data->textu_img[0], data->texinfo.north);
	init_texture_img(data, &data->textu_img[1], data->texinfo.south);
	init_texture_img(data, &data->textu_img[2], data->texinfo.east);
	init_texture_img(data, &data->textu_img[3], data->texinfo.west);
	data->textures[0] = xpm_to_img(data, data->texinfo.north);
	data->textures[1] = xpm_to_img(data, data->texinfo.south);
	data->textures[2] = xpm_to_img(data, data->texinfo.east);
	data->textures[3] = xpm_to_img(data, data->texinfo.west);
}

//inicializamos las vars del struct de texturas
void	init_texstruct(t_texinfo *textures)
{
	textures->north = NULL;
	textures->south = NULL;
	textures->west = NULL;
	textures->east = NULL;
	textures->floor = -1;
	textures->ceiling = -1;
	textures->size = TEXTURE_SIZE;
}
