/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill_env_cmd_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnicolas <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 20:25:30 by jnicolas          #+#    #+#             */
/*   Updated: 2022/12/12 21:52:35 by jnicolas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../my_shell.h"

void remove_double_quotes(char *str)
{
  int i;
  // printf("<%s> len is %d\n",str,strlen(str));
  if ((str[0] == '\'' || str[0] == '"') && str[0] == str[strlen(str) - 1])
  {
    for (i = 0; i < (strlen(str) - 2); i++)
    {
      str[i] = str[i + 1];
    }
    str[i] = '\0';
  }
}

int correct_envvar_char(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

char	*fill_env_token(char *token)
{
	if (token[0] != '\'')
	{
		while (1)
		{
			int i = 0;
			while (token[i] != '$' && token[i])
				i++;
			if (token[i])
			{
				int j = i + 1;
				while (correct_envvar_char(token[j]) && token[j])
					j++;
				j = j - 1;
				if (i == j)
				{
					return "$";
				}
				i++;
				int k = 0;
				char *envvar = malloc(sizeof(char) * (j - i + 2));
				int envStart = i - 1;
				int envEnd = j;
				while (i <= j)
					envvar[k++] = token[i++];
				envvar[k] = '\0';
				char *envval = getenv(envvar);
				if (envval == NULL)
				{
					envval = (char *)malloc(sizeof(char));
					*envval = '\0';
				}
				int newTokenLen = strlen(token) - strlen(envvar) - 1 + strlen(envval);
				char *newTokenVal = (char *)malloc(sizeof(char) * newTokenLen);
				newTokenVal[0] = '\0';
				strncpy(newTokenVal, token, envStart + 1);
				newTokenVal[envStart] = '\0';
				strcat(newTokenVal, envval);
				if (token[envEnd + 1])
					strcat(newTokenVal, &(token[envEnd + 1]));
				free(token);
				remove_double_quotes(newTokenVal);
				return newTokenVal;
			}
			else
			{
				remove_double_quotes(token);
				return token;
			}
		}
	}
	else
	{
		remove_double_quotes(token);
		return token;
	}
}

void	fill_env_cmd(t_parsed_cmd *cmd)
{
	cmd->name = fill_env_token(cmd->name);
	if (cmd->arguments)
	{
		string_list *arglist = cmd->arguments;
		while (arglist != NULL)
		{
			arglist->string = fill_env_token(arglist->string);
			arglist = arglist->next;
		}
	}
	if (cmd->redirections != NULL)
	{
		redirect_list *ptr_redir = cmd->redirections;
		while (ptr_redir != NULL)
		{
			ptr_redir->source = fill_env_token(ptr_redir->source);
			ptr_redir = ptr_redir->next;
		}
	}
}

void	fill_env_cmd_list(t_parsed_cmd_list *parsed_cmd_list)
{
	while (parsed_cmd_list != NULL)
	{
		fill_env_cmd(parsed_cmd_list->command);
		parsed_cmd_list = parsed_cmd_list->next;
	}
}
