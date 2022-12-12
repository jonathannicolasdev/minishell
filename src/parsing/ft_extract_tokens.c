/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extract_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnicolas <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 20:22:51 by jnicolas          #+#    #+#             */
/*   Updated: 2022/12/12 17:57:34 by jnicolas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../my_shell.h"

string_list	*create_token(char *value)
{
	string_list	*token;

	token = (string_list *)malloc(sizeof(string_list));
	token->string = value;
	return (token);
}

int	redirect_position(char *str, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (str[i] == '<' || str[i] == '>' || str[i] == '|' || str[i] == ';')
			return (i);
		i++;
	}
	return (-1);
}

string_list	*recursive_extract_tokens(char	*text)
{
	string_list	*token;
	char		stop_char;
	char		*token_value;
	int			quoted;
	int			tk_len;
	int			posredir;
	int			redir_len;

	tk_len = 0;
	while (*text == ' ')
		text++;
	if (!*text)
		return (NULL);
	if (*text == '\'' || *text == '"')
	{
		quoted = 1;
		stop_char = *text;
	}
	else
		quoted = 0;
	if (quoted)
	{
		tk_len = 1;
		while (text[tk_len] != stop_char && text[tk_len])
			tk_len++;
		tk_len++;
	}
	else
	{
		tk_len = 0;
		while (text[tk_len] != ' ' && text[tk_len])
			tk_len++;
		posredir = redirect_position(text, tk_len);
		if (posredir != -1)
		{
			redir_len = 1;
			if (text[posredir] == text[posredir + 1])
				redir_len = 2;
			if (posredir == 0)
				tk_len = redir_len;
			else
				tk_len = posredir;
		}
	}
	token_value = (char *)malloc(sizeof(char) * (tk_len + 1));
	strncpy(token_value, text, tk_len);
	token_value[tk_len] = '\0';
	token = create_token(token_value);
	token->next = recursive_extract_tokens(text + tk_len);
	return (token);
}
