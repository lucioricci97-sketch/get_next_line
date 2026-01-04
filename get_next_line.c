/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luricci <luricci@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 17:15:25 by luricci           #+#    #+#             */
/*   Updated: 2026/01/04 19:41:35 by luricci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_file(int fd, char *stash)
{
	char	*buffer;
	int		byte_read;

	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (NULL);
	byte_read = 1;
	while (!ft_strchr(stash, '\n') && byte_read != 0)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
		{
			free(buffer);
			free(stash);
			return (NULL);
		}
		buffer[byte_read] = '\0';
		stash = ft_strjoin(stash, buffer);
	}
	free(buffer);
	return (stash);
}

char	*ft_line(char *stash)
{
	char	*line;
	int		len;
	int		j;

	len = 0;
	j = 0;
	if (!stash || !stash[j])
		return (NULL);
	while (stash[len] && stash[len] != '\n')
		len++;
	line = ft_calloc((len + 2), sizeof(char));
	if (!line)
		return (NULL);
	while (stash[j] && stash[j] != '\n')
	{
		line[j] = stash[j];
		j++;
	}
	if (stash[j] && stash[j] == '\n')
		line[j++] = '\n';
	line[j] = '\0';
	return (line);
}

char	*ft_next(char *stash)
{
	char	*rest;
	int		len;
	int		j;

	len = 0;
	j = 0;
	while (stash[len] && stash[len] != '\n')
		len++;
	if (!stash[len])
	{
		free(stash);
		return (NULL);
	}
	rest = ft_calloc((ft_strlen(stash) - len + 1), sizeof(char));
	if (!rest)
	{
		free(stash);
		return (NULL);
	}
	len++;
	j = 0;
	while (stash[len])
		rest[j++] = stash[len++];
	free(stash);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_file(fd, stash);
	if (!stash)
		return (NULL);
	line = ft_line(stash);
	stash = ft_next(stash);
	return (line);
}

/*
int main()
{
	int     fd;
	char    *line;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
*/