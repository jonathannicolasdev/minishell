/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnicolas <jnicolas@student.42quebec>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/03 15:19:53 by jnicolas          #+#    #+#             */
/*   Updated: 2021/11/07 21:55:21 by jnicolas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_wordcount(char const	*str, int c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == c)
			i++;
		if (str[i] && str[i] != c)
			count++;
		while (str[i] && str[i] != c)
			i++;
	}
	return (count);
}

size_t	ft_wordlen(char const *str, int c)
{
	size_t	len;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	return (len);
}

char	*ft_copy(char const *s, size_t len)
{
	char	*str;
	size_t	i;

	i = 0;
	str = (char *)malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	wordcount;
	size_t	wordlen;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	wordcount = ft_wordcount(s, c);
	res = malloc(sizeof(char *) * (wordcount + 1));
	if (!res)
		return (NULL);
	while (i < wordcount)
	{
		wordlen = ft_wordlen(s, c);
		if (wordlen)
		{
			res[i++] = ft_copy(s, wordlen);
			s += wordlen + 1;
		}
		else
			s += 1;
	}
	res[i] = NULL;
	return (res);
}
