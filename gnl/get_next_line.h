/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnicolas <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 21:32:15 by jnicolas          #+#    #+#             */
/*   Updated: 2021/12/16 17:57:32 by jnicolas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>

//size_t	ft_strlen(char *s);
//char	*ft_strjoin(char *s1, char *s2);
//char	*ft_strchr(char *s, int c);
int		ft_strendl(char *s);
char	*ft_strnew(size_t size);
char	*get_line(char *memory);
char	*get_remainder(char *memory);
char	*ft_read(int fd, char *memory);
char	*get_next_line(int fd);

# define BUFFER_SIZE 42

#endif
