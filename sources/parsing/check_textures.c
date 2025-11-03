/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 17:31:33 by cmarrued          #+#    #+#             */
/*   Updated: 2025/09/21 18:22:22 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//Check that files for textures are valid and save them
int	check_textures_validity(t_data *data, t_texinfo *textures)
{
	if (!textures->north || !textures->south || !textures->west
		|| !textures->east)
		return (err_msg(data->mapinfo.path, ERR_TEX_MISSING, FAILURE));
	if (textures->floor == -1 || textures->ceiling == -1)
		return (err_msg(data->mapinfo.path, ERR_COLOR_MISSING, FAILURE));
	if (check_file(textures->north, false) == FAILURE
		|| check_file(textures->south, false) == FAILURE
		|| check_file(textures->west, false) == FAILURE
		|| check_file(textures->east, false) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
