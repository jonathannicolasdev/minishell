/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnicolas <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 21:27:03 by jnicolas          #+#    #+#             */
/*   Updated: 2022/10/31 17:31:28 by jnicolas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "../libft/libft.h"

char	*get_line(char *memory)
{
	int		i;
	char	*line;

	i = 0;
	if (!memory[i])
		return (NULL);
	while (memory[i] && memory[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (memory[i] && memory[i] != '\n')
	{
		line[i] = memory[i];
		i++;
	}
	if (memory[i] == '\n')
	{
		line[i] = memory[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*get_remainder(char *memory)
{
	int		i;
	int		j;
	char	*remainder;

	i = 0;
	while (memory[i] && memory[i] != '\n')
		i++;
	if (!memory[i])
	{
		free(memory);
		return (NULL);
	}
	remainder = malloc(sizeof(char) * (ft_strlen(memory) - i + 1));
	if (!remainder)
		return (NULL);
	j = 0;
	i++;
	while (memory[i])
		remainder[j++] = memory[i++];
	remainder[j] = '\0';
	free(memory);
	return (remainder);
}

char	*ft_read(int fd, char *memory)
{
	char	*buff;
	int		read_bytes;
	char	*temp;

	buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (NULL);
	read_bytes = 1;
	while (!ft_strendl(memory) && read_bytes != 0)
	{
		read_bytes = read(fd, buff, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(buff);
			return (NULL);
		}
		if (!memory)
			memory = ft_strnew(0);
		buff[read_bytes] = '\0';
		temp = memory;
		memory = ft_strjoin(temp, buff);
		free(temp);
	}
	free(buff);
	return (memory);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*memory;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	memory = ft_read(fd, memory);
	if (!memory)
		return (NULL);
	line = get_line(memory);
	memory = get_remainder(memory);
	return (line);
}
