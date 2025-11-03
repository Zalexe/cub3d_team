/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intherna <intherna@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 18:53:00 by intherna          #+#    #+#             */
/*   Updated: 2025/10/23 22:00:01 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	abs_i(int i)
{
	return ((-(i < 0) * i) + (i >= 0) * i);
}

/**
 * Gets the sign of an integer.
 *
 * Returns -1 if i < 0, +1 otherwise.
 */
int	sign_i(int i)
{
	return (-(i < 0) + (i >= 0));
}

int	max_i(int i1, int i2)
{
	return (((i1 >= i2) * i1) + ((i2 > i1) * i2));
}

int	min_i(int i1, int i2)
{
	return (((i1 <= i2) * i1) + ((i2 < i1) * i2));
}

/**
 * Finds the smallest difference between 2 angles
 */
double	angle_diff(double rads1, double rads2)
{
	rads1 += M_PI;
	rads2 += M_PI;
	return (min_d((2 * M_PI) - abs_d(rads1 - rads2), abs_d(rads1 - rads2)));
}
