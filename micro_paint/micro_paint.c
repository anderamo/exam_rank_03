/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamorin- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 09:50:02 by aamorin-          #+#    #+#             */
/*   Updated: 2021/09/30 11:30:50 by aamorin-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct s_map
{
	int		w;
	int		h;
	char	c;
}	t_map;

typedef struct s_paint
{
	char		type;
	float		w;
	float		h;
	float		x;
	float		y;
	char		c;
}	t_paint;

int	ft_freetab(char **paint)
{
	int i;

	i = 0;
	if (paint[i])
	{
		free(paint[i]);
		i++;
	}
	free(paint);
	return (1);
}

int	ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int ft_put_arraybi(char **paint)
{
	int i;

	i = 0;
	while(paint[i])
	{
		write(1, paint[i], ft_strlen(paint[i]));
		write(1, "\n", 1);
		i++;
	}
	return (1);
}

int check_in_rectangle(float x, float y, t_paint paint)
{
	float xbr;
	float ybr;

	xbr = paint.x + paint.w;
	ybr = paint.y + paint.h;
	if (x >= paint.x && x <= xbr && y >= paint.y && y <= ybr)
	{
		if (x - paint.x < 1 || xbr - x < 1 || y - paint.y < 1|| ybr - y < 1)
			return (0);
		return (1);
	}
	return (-1);
}

void paint_rectangle(t_map map, t_paint paint, char **arraybi)
{
	int i;
	int j;
	int k;

	i = 0;
	while (map.h > i)
	{
		j = 0;
		while (map.w > j)
		{
			k = check_in_rectangle(j, i, paint);
			if (k == 0 || (k == 1 && paint.type == 'R'))
				arraybi[i][j] = paint.c;
			j++;
		}
		i++;
	}
}

int	background(FILE *fd, t_map map)
{
	char	**arraybi;
	int		i;
	int		ret;
	t_paint	paint;

	arraybi = (char **)malloc(sizeof(char *) * (map.h + 1));
	if (!arraybi)
		return (1);
	i = 0;
	while (map.h > i)
	{
		arraybi[i] = (char *)malloc(sizeof(char) * (map.w + 1));
		memset(arraybi[i], map.c, map.w);
		arraybi[i][map.w] = '\0';
		i++;
	}
	arraybi[i] = 0;
	while ((ret = fscanf(fd, "%c %f %f %f %f %c\n", &paint.type, &paint.x, &paint.y, &paint.w, &paint.h, &paint.c)) == 6)
	{
		if ((paint.type != 'R' && paint.type != 'r') || paint.w <= 0.0
			|| paint.h <= 0.0 || paint.c == 0 || paint.c == '\n')
			return(ft_freetab(arraybi));
		paint_rectangle(map, paint, arraybi);
	}
	if (ret != -1)
		return (ft_freetab(arraybi));
	ft_put_arraybi(arraybi);
	ft_freetab(arraybi);
	return (0);
}


int	get_map_info(FILE *fd, t_map *map)
{
	int ret;

	ret = fscanf(fd, "%d %d %c\n", &map->w, &map->h, &map->c);
	if (map->w <= 0 || map->w > 300 || map->h <= 0 || map->h > 300
		|| map->c == 0 || map->c == '\n')
		return (1);
	return (0);
}


int main(int a, char **argv)
{
	FILE	*fd;
	t_map	map;

	if (a != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if (!(fd = fopen(argv[1],"r")))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	if (get_map_info(fd, &map) == 1)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	if (background(fd, map) == 1)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	fclose(fd);
	return (0);
}
