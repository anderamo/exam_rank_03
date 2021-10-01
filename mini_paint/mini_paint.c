/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamorin- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 11:31:20 by aamorin-          #+#    #+#             */
/*   Updated: 2021/09/30 12:20:48 by aamorin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

typedef struct s_map
{
	int		w;
	int		h;
	char 	c;
}	t_map;

typedef struct s_paint
{
	char type;
	float x;
	float y;
	float radius;
	char c;
} t_paint;

int	ft_freetab(char **arraybi)
{
	int i;

	i = 0;
	while (arraybi[i])
	{
		free(arraybi[i]);
		i++;
	}
	free(arraybi);
	return (1);
}

int check_in_circle(float x, float y, t_paint paint)
{
	float distance;

	distance = sqrtf(((x - paint.x) * (x - paint.x)) + ((y - paint.y) * (y - paint.y)));
	if (distance <= paint.radius)
	{
		if (paint.radius - distance < 1)
			return (2);
		return (1);
	}
	return (0);
}

void	paint_circle(t_map map, t_paint paint, char **arraybi)
{
	int i;
	int k;
	int j;

	i = 0;
	while (i < map.h)
	{
		j = 0;
		while (j < map.w)
		{
			k = check_in_circle(j, i, paint);
			if ((paint.type == 'c' && k == 2) || (k && paint.type == 'C'))
				arraybi[i][j] = paint.c;
			j++;
		}
		i++;
	}
}

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void ft_put_arraybi(char **arraybi)
{
	int i;

	i = 0;
	while (arraybi[i])
	{
		write(1, arraybi[i], ft_strlen(arraybi[i]));
		write(1, "\n", 1);
		i++;
	}
}

int background(FILE *fd, t_map map)
{
	int ret;
	int i;
	t_paint paint;
	char **arraybi;

	arraybi = (char **)malloc(sizeof(char *) * (map.h + 1));
	i = 0;
	while (i < map.h)
	{
		arraybi[i] = (char *)malloc((sizeof(char) * (map.w + 1)));
		memset(arraybi[i], map.c, map.w);
		arraybi[i][map.w] = '\0';
		i++;
	}
	arraybi[i] = 0;
	while ((ret = fscanf(fd, "%c %f %f %f %c\n", &paint.type, &paint.x, &paint.y, &paint.radius, &paint.c)) == 5)
	{
		if ((paint.type != 'C' && paint.type != 'c') || paint.radius <= 0)
			return (ft_freetab(arraybi));
			paint_circle(map, paint, arraybi);
	}
	if (ret != -1)
		return (ft_freetab(arraybi));
	ft_put_arraybi(arraybi);
	ft_freetab(arraybi);
	return (0);
}

int get_map_values(FILE *fd, t_map *map)
{
	int ret;

	ret = fscanf(fd, "%d %d %c\n", &map->w, &map->h, &map->c);
	if (!ret || map->w > 300 || map->h > 300 || map->h <= 0 || map->w <= 0 || map->c == 0 || map->c == '\n')
		return (1);
	return (0);

}

int main(int a, char **argv)
{
	FILE *fd;
	t_map map;

	if (a != 2)
	{
		write(1, "Error: argument\n" ,16);
		return (1);
	}
	if (!(fd = fopen(argv[1], "r")))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	if (get_map_values(fd, &map) == 1)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	if (background(fd, map) == 1)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
}
