/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jin-tan <jin-tan@student.42kl.edu.my>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 20:41:17 by jin-tan           #+#    #+#             */
/*   Updated: 2024/08/04 07:09:16 by jin-tan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Storage hierachy: buffer, stash, line

// read_bytes is a necessary implementation of BUFFER_SIZE,
// as our program should not be reading the same size all the time,
// due to limitations like newline.

// before completing read_to_stash,
// buffer is null-terminated (to make it a valid string)
// and also concatenated to stash

char	*read_to_stash(int fd, char *stash)
{
	char	*buff;
	int		read_bytes;

	buff = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff)
		return (NULL);
	read_bytes = 1;
	while (!ft_strchr(stash, '\n') && read_bytes != 0)
	{
		read_bytes = read(fd, buff, BUFFER_SIZE);
		if (read_bytes == -1)
		{
			free(buff);
			return (NULL);
		}
		buff[read_bytes] = '\0';
		stash = ft_strjoin(stash, buff);
	}
}

// Extracts line from stash to line
// i + 2 accounts for potential newline and null terminator

char	*read_to_line(char *stash)
{
	int		i;
	char	*str;

	i = 0;
	if (!stash)
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	str = malloc((i + 2) * sizeof(char));
	if (!str)
		return (NULL);
	while (stash[i] && stash[i] != '\n')
	{
		str[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
	{
		str[i] = stash[i];
		i++;
	}
	str[i] == '\0';
	return (str);
}

// updates stash by removing the line extracted by read_to_line
// this will creatw two stashes, then discard old stash and return new stash

// if while loop reaches end of string without newline,
// entire string was processed, thus stash is memory freed.

// when encountered newline as you copy stash to str,
// do not copy before newline
// hence, malloc will be length of string
// - newline index + 1 for null terminator

// Old stash: Hello\nWorld!\nThis is a test.\n
// New stash: "World!\nThis is a test.\n"

char	*update_stash(char *stash)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	str = malloc(ft_strlen(stash) - i + 1 * sizeof(char));
	if (!str)
		return (NULL);
	i++;
	while (stash[i])
	{
		str[j++] = stash[i++];
		str[j] = '\0';
		free(stash);
		return (str);
	}
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_to_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = read_to_line(stash);
	stash = update_stash(stash);
	return (line);
}
