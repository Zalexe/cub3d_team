/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmarrued <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:33:54 by cmarrued          #+#    #+#             */
/*   Updated: 2025/10/01 17:13:57 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//print controls in console
static void	print_controls(void)
{
	printf("\n");
	printf("\t\t\tCUB3D CONTROLS\t");
	printf("\n");
	printf("\tW: move forward\t");
	printf("\t\tS: move backward\n");
	printf("\tA: strafe left\t");
	printf("\t\tD: strafe right\n");
	printf("\tLeft arrow: rotate left\t");
	printf("\tRight arrow: rotate right\n");
	printf("\n");
}

//several checks in /parsing
static int	parse_args(t_data *data, char **av)
{
	if (check_file(av[1], true) == FAILURE)
		clean_exit(data, FAILURE);
	parse_data(av[1], data);
	if (get_file_data(data, data->mapinfo.file) == FAILURE)
		return (free_data(data));
	if (check_map_validity(data, data->map) == FAILURE)
		return (free_data(data));
	if (check_textures_validity(data, &data->texinfo) == FAILURE)
		return (free_data(data));
	init_player_direction(data);
	return (SUCCESS);
}

int	main(int arg, char **av)
{
	t_data	data;

	if (arg != 2)
		return (err_msg("Usage", ERR_USAGE, 1));
	init_data(&data);
	if (parse_args(&data, av) != 0)
		return (1);
	init_mlx(&data);
	init_textures(&data);
	print_controls();
	fill(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.canvas.img, 0, 0);
	listen_input(&data);
	mlx_loop_hook(data.mlx, render, &data);
	mlx_loop(data.mlx);
	return (SUCCESS);
}
