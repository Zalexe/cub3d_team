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

static int	check_str_num(char *str)
{
	char	*limit;
	int		found_greater;

	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	if (!ft_isdigit(*str))
		return (0);
	limit = "2147483647";
	found_greater = 0;
	while (*limit && *str && *str != ' ')
	{
		if (!found_greater && *str > *limit)
			found_greater = 1;
		str++;
		limit++;
	}
	if (!ft_isdigit(*str))
		return (0);
	if (*str == ' ' && *limit)
		return (1);
	if (*limit == '\0' && (found_greater || ft_isdigit(*str)))
		return (0);
	return (1);
}

static bool	no_digit(char *str)
{
	int		i;
	bool	found_no_digit;

	i = 0;
	found_no_digit = true;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 1)
			found_no_digit = false;
		i++;
	}
	return (found_no_digit);
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
		if (no_digit(rgb_to_convert[i])
			|| !check_str_num(rgb_to_convert[i]) || tmp > 255)
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
