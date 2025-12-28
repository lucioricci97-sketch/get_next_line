/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luricci <luricci@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:10:08 by luricci           #+#    #+#             */
/*   Updated: 2025/12/28 18:47:10 by luricci          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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
	char	*line;
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
	line = ft_calloc((ft_strlen(stash) - len + 1), sizeof(char));
	if (!line)
	{
		free(stash);
		return (NULL);
	}
	len++;
	j = 0;
	while (stash[len])
		line[j++] = stash[len++];
	free(stash);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash[1024];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)
		return (NULL);
	stash[fd] = read_file(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = ft_line(stash[fd]);
	stash[fd] = ft_next(stash[fd]);
	return (line);
}

/*
int main()
{
    int     fd1, fd2;
    char    *line1, *line2;
    int     count1, count2;

    count1 = 0;
    count2 = 0;
    // Open two files (ensure test1.txt and test2.txt exist)
    fd1 = open("test1.txt", O_RDONLY);
    fd2 = open("test2.txt", O_RDONLY);
    if (fd1 == -1 || fd2 == -1)
    {
        printf("Error opening file");
        return (1);
    }

    printf("---- START READING ----\n");

    // Call get_next_line in a loop for both files
    while (1)
    {
        line1 = get_next_line(fd1);
        line2 = get_next_line(fd2);
        if (line1 == NULL && line2 == NULL)
            break;
        if (line1)
        {
            count1++;
            printf("[File 1 - %d]: %s", count1, line1);
            free(line1);
        }
        if (line2)
        {
            count2++;
            printf("[File 2 - %d]: %s", count2, line2);
            free(line2);
        }
    }

    // Close files and finish
    close(fd1);
    close(fd2);
    printf("---- END OF FILES ----\n");
    return (0);
}
*/