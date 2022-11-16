/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnicolas <jnicolas@student.42quebec>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/09 15:46:49 by jnicolas          #+#    #+#             */
/*   Updated: 2021/11/07 22:32:53 by jnicolas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static void	ft_pop_new(int i, long nb, char *new)
{
	while (nb > 0)
	{
		new[i] = nb % 10 + '0';
		nb = nb / 10;
		i--;
	}
}

char	*ft_itoa(int n)
{
	char	*new;
	long	nb;
	int		len;
	int		i;

	nb = (long)n;
	len = ft_len(n);
	i = len - 1;
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	if (nb == 0)
	{
		new[0] = '0';
		new[1] = '\0';
		return (new);
	}
	if (n < 0)
	{
		nb = nb * -1;
		new[0] = '-';
	}
	ft_pop_new(i, nb, new);
	new[len] = '\0';
	return (new);
}
