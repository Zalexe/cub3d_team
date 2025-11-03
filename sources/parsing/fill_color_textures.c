/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_color_textures.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 17:31:42 by cmarrued          #+#    #+#             */
/*   Updated: 2025/09/21 18:58:56 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	no_digit(char *str)
{
	int		counter;

	counter = 0;
	while (*str && (*str == ' ' || *str == '\t'
			|| *str == '\r'
			|| *str == '\v' || *str == '\f' || *str == '0'))
		str++;
	while (*str && (*str != ' ' && *str != '\t'
			&& *str != '\r'
			&& *str != '\v' && *str != '\f' && *str != '\n'))
	{
		if (counter > 3 || !ft_isdigit(*str))
			return (true);
		counter++;
		str++;
	}
	while (*str && (*str == ' ' || *str == '\t'
			|| *str == '\r'
			|| *str == '\v' || *str == '\f' || *str == '\n'))
		str++;
	if (*str)
		return (true);
	return (false);
}

static uint32_t	extract_rgb(char **rgb_to_convert)
{
	int			i;
	uint32_t	rgb;
	uint32_t	tmp;

	i = -1;
	rgb = 0x00000000;
	while (rgb_to_convert[++i])
	{
		tmp = ft_atoi(rgb_to_convert[i]);
		if (no_digit(rgb_to_convert[i]) || tmp > 255)
		{
			free_tab((void **)rgb_to_convert);
			return (-1);
		}
		rgb |= ((tmp & 0x000000FF) << (16 - 8 * i));
	}
	free_tab((void **)rgb_to_convert);
	return (rgb);
}

static uint32_t	get_rgb_colors(char *line)
{
	char		**rgb_to_convert;
	int			count;

	rgb_to_convert = ft_split(line, ',');
	count = 0;
	while (rgb_to_convert[count])
		count++;
	if (count != 3)
	{
		free_tab((void **)rgb_to_convert);
		return (-1);
	}
	return (extract_rgb(rgb_to_convert));
}

int	fill_color_textures(t_data *data, t_texinfo *textures, char *line, int j)
{
	if (line[j + 1] && ft_isprint(line[j + 1]))
		return (err_msg(data->mapinfo.path, ERR_FLOOR_CEILING, ERR));
	if (textures->ceiling == -1 && line[j] == 'C')
	{
		textures->ceiling = get_rgb_colors(line + j + 1);
		if (textures->ceiling == -1)
			return (err_msg(data->mapinfo.path, ERR_COLOR_CEILING, ERR));
	}
	else if (textures->floor == -1 && line[j] == 'F')
	{
		textures->floor = get_rgb_colors(line + j + 1);
		if (textures->floor == -1)
			return (err_msg(data->mapinfo.path, ERR_COLOR_FLOOR, ERR));
	}
	else
		return (err_msg(data->mapinfo.path, ERR_FLOOR_CEILING, ERR));
	return (SUCCESS);
}
