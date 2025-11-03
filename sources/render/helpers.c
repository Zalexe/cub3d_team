/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intherna <intherna@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:01:56 by intherna          #+#    #+#             */
/*   Updated: 2025/10/23 22:04:49 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Gets the pixel from in image at a specified location in pixels
 */
void	*get_pixel_addr_at(t_image *img, uint32_t x, uint32_t y)
{
	return (((unsigned char *)img->addr)
		+ (y * img->size_line + x * (img->pixel_bits / 8)));
}

/**
 * Gets the pixel from in image at a specified location in pixels.
 * Pixels pointer being separated from the image data, because
 * that's how we have our textures stored.
 */
void	*iget_pixel_addr_at(uint32_t *img, t_image *in, uint32_t x, uint32_t y)
{
	return (((char *)img) + (y * in->size_line + x * (in->pixel_bits / 8)));
}

void	paint_static_color(uint32_t color, void *p, int x, int y)
{
	uint64_t	*addr;
	size_t		i;
	uint64_t	color_64;
	size_t		len;

	addr = (uint64_t *)p;
	i = 0;
	len = x * y;
	color_64 = (((uint64_t)color) << 32) | color;
	while (i < len / 2)
	{
		*addr = color_64;
		addr++;
		i++;
	}
	if (len % 2 != 0)
		*((uint32_t *)addr) = color;
}

t_dir	get_dir_from_rads(double rads)
{
	t_dir	res;

	res.rads = rads;
	res.x = cos(res.rads);
	res.y = sin(res.rads);
	return (res);
}

t_dir	get_dir(t_data *data, t_point *p)
{
	return (get_dir_from_rads(atan2(p->y
				- data->player.pos_y, p->x - data->player.pos_x)));
}
