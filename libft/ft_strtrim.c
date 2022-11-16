/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnicolas <jnicolas@student.42quebec>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 21:37:40 by jnicolas          #+#    #+#             */
/*   Updated: 2021/11/09 13:50:07 by jnicolas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnicolas <jnicolas@student.42quebec>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/29 19:29:17 by jnicolas          #+#    #+#             */
/*   Updated: 2021/10/31 22:05:11 by jnicolas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
//#include <stdio.h>
//#include <stdlib.h>

#include "libft.h"

size_t	ft_isinset(const char *set, int c)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*trim;
	int		i;
	int		len;
	int		start;
	int		end;

	start = 0;
	if (!s1)
		return (NULL);
	while (s1[start] && ft_isinset(set, s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_isinset(set, s1[end - 1]))
		end--;
	len = end - start + 1;
	trim = (char *)malloc(sizeof(*s1) * len);
	if (trim == NULL)
		return (NULL);
	i = 0;
	while (start < end)
	{
		trim[i++] = s1[start++];
	}
	trim[i] = '\0';
	return (trim);
}
