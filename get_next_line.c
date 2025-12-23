/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luricci <luricci@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 17:15:25 by luricci           #+#    #+#             */
/*   Updated: 2025/12/23 16:03:26 by luricci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_file(int fd, char *res)
{
	char	*buffer;
	int		byte_read;

	buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
	if (!buffer)
		return (NULL);
	byte_read = 1;
	while (!ft_strchr(res, '\n') && byte_read != 0)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
		{
			free(buffer);
			free(res);
			return (NULL);
		}
		buffer[byte_read] = '\0';
		res = ft_strjoin(res, buffer);
	}
	free(buffer);
	return (res);
}

char	*ft_line(char *buffer)
{
	char	*line;
	int		len;
	int		j;

	len = 0;
	j = 0;
	if (!buffer || !buffer[j])
		return (NULL);
	while (buffer[len] && buffer[len] != '\n')
		len++;
	line = ft_calloc((len + 2), sizeof(char));
	if (!line)
		return (NULL);
	while (buffer[j] && buffer[j] != '\n')
	{
		line[j] = buffer[j];
		j++;
	}
	if (buffer[j] && buffer[j] == '\n')
		line[j++] = '\n';
	line[j] = '\0';
	return (line);
}

char	*ft_next(char *buffer)
{
	char	*line;
	int		len;
	int		j;

	len = 0;
	j = 0;
	while (buffer[len] && buffer[len] != '\n')
		len++;
	if (!buffer[len])
	{
		free(buffer);
		return (NULL);
	}
	line = ft_calloc((ft_strlen(buffer) - len + 1), sizeof(char));
	len++;
	j = 0;
	while (buffer[len])
		line[j++] = buffer[len++];
	free(buffer);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_file(fd, buffer);
	if (!buffer)
		return (NULL);
	line = ft_line(buffer);
	buffer = ft_next(buffer);
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