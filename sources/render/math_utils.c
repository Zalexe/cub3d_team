/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: intherna <intherna@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 18:50:12 by intherna          #+#    #+#             */
/*   Updated: 2025/10/23 22:01:01 by intherna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	max_d(double d1, double d2)
{
	return (((d1 >= d2) * d1) + ((d2 > d1) * d2));
}

double	min_d(double d1, double d2)
{
	return (((d1 <= d2) * d1) + ((d2 < d1) * d2));
}

/**
 * Gets the sign of a double.
 *
 * Returns -1.0 if i < 0, +1.0 otherwise.
 */
double	sign(double d)
{
	return (-(d < 0) + (d >= 0));
}

double	abs_d(double d)
{
	return ((-(d < 0) * d) + (d >= 0) * d);
}

/**
 * Calculates the world distance between 2 points in the map.
 */
double	diff(t_point p1, t_point p2)
{
	double	dx;
	double	dy;

	dx = abs_d(p2.x - p1.x);
	dy = abs_d(p2.y - p1.y);
	return (sqrt((dx * dx) + (dy * dy)));
}
