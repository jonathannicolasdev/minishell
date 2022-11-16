/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnicolas <jnicolas@student.42quebec>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 18:48:38 by jnicolas          #+#    #+#             */
/*   Updated: 2021/11/09 11:55:06 by jnicolas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	current_start;

	i = 0;
	if (needle[i] == '\0')
	{
		return ((char *)haystack);
	}
	while (haystack[i] && i < len)
	{
		current_start = i;
		j = 0;
		while (haystack[current_start] == needle[j] && current_start < len)
		{
			if (needle[j + 1] == '\0')
				return ((char *)&haystack[i]);
			current_start++;
			j++;
		}
		i++;
	}
	return (NULL);
}
