/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_create_parsed_cmd_list.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnicolas <marvin@42quebec.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 18:06:08 by jnicolas          #+#    #+#             */
/*   Updated: 2022/12/12 21:34:31 by jnicolas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../my_shell.h"

t_parsed_cmd	*create_init_parsed_cmd(void)
{
	t_parsed_cmd	*parsed_cmd;

	parsed_cmd = (t_parsed_cmd *)malloc(sizeof(t_parsed_cmd));
	parsed_cmd->name = (char *) NULL;
	parsed_cmd->arguments = (string_list *) NULL;
	parsed_cmd->redirections = (redirect_list *) NULL;
	parsed_cmd->is_piped = 0;
	return (parsed_cmd);
}

string_list	*create_arg_from_token(char *value)
{
	string_list *arg;
	arg = (string_list *)malloc(sizeof(string_list));
	arg->string = strdup(value);
	arg->next = NULL;
	return (arg);
}

int	is_redirect_token(char *c, int token_size)
{
	if (token_size == 1)
	{
		return (*c == '<' || *c == '>' || *c == '|' || *c == ';');
	}
	if (token_size == 2)
	{
		if (strlen(c) != 2)
			return (0);
		return ((*c == '<' || *c == '>') && *c == *++c);
	}
	return (0);
}

int	redirect_token_type(char *c)
{
	if (strlen(c) == 1)
	{
		switch (*c)
		{
			case '<':
				return (INPUT_FILE);
			case '>':
				return (OUTPUT_FILE_CREATE);
		}
	}
	if (strlen(c) == 2)
	{
		if ((*c == '<') && *(c + 1) == '<')
			return (INPUT_NEXT_LINE);
		if ((*c == '>') && *(c + 1) == '>')
			return (OUTPUT_FILE_APPEND);
	}
	return (NO_REDIR);
}

redirect_list	*append_new_redirect(redirect_list **redir, char *source, t_io_direction direction)
{
	redirect_list	*cell;
	redirect_list	*ptr;

	cell = (redirect_list *)malloc(sizeof(redirect_list));
	cell->source = source;
	cell->direction = direction;
	cell->next = NULL;
	if (*redir == NULL)
	{
		*redir = cell;
		return (cell);
	}
	ptr = *redir;
	while (ptr->next != NULL)
	{
		ptr = ptr->next;
	}
	ptr->next = cell;
	return (cell);
}

int	parse_fill_parsed_cmd(t_parsed_cmd *parsed_cmd, string_list *from_token, \
		string_list *to_token, char separator)
{
	string_list		*tokens;
	string_list		*ptr_arg;
	redirect_list	*ptr_redir;
	string_list		*arg;

	ptr_arg = NULL;
	ptr_redir = NULL;
	if (!is_redirect_token(from_token->string, 1))
	{
		parsed_cmd->name = strdup(from_token->string);
		if (from_token == to_token)
			return (1);
		from_token = from_token->next;
	}
	while (from_token != NULL)
	{
		if (redirect_token_type(from_token->string) != NO_REDIR)
		{
			if (from_token->next == NULL)
			{
				perror("syntax error near unexpected token `newline'");
				return (-1);
			}
			if (redirect_token_type(from_token->next->string) != NO_REDIR)
			{
				printf("syntax error near unexpected token `%c'", \
						from_token->next->string[0]);
				return (-1);
			}
			ptr_redir = append_new_redirect(&(parsed_cmd->redirections), \
					strdup(from_token->next->string), \
					redirect_token_type(from_token->string));
			from_token = from_token->next;
			if (from_token == to_token)
				break ;
		}
		else
		{
			arg = create_arg_from_token(from_token->string);
			if (parsed_cmd->arguments == NULL)
			{
				parsed_cmd->arguments = arg;
				ptr_arg = arg;
			}
			else
			{
				ptr_arg->next = arg;
				ptr_arg = ptr_arg->next;
			}
		}
		if (from_token == to_token)
			break ;
		from_token = from_token->next;
	}
	return (1);
}

t_parsed_cmd_list	*append_new_pcl(t_parsed_cmd_list **parsed_cmd_list, \
		t_parsed_cmd *parsed_cmd)
{
	t_parsed_cmd_list	*cell;
	t_parsed_cmd_list	*ptr;

	cell = (t_parsed_cmd_list *)malloc(sizeof(t_parsed_cmd_list));
	cell->command = parsed_cmd;
	cell->next = NULL;
	if (*parsed_cmd_list == NULL)
	{
		*parsed_cmd_list = cell;
		return (cell);
	}
	ptr = *parsed_cmd_list;
	while (ptr->next != NULL)
	{
		ptr = ptr->next;
	}
	ptr->next = cell;
	return (cell);
}

t_parsed_cmd_list	*create_parsed_cmd_list(string_list *tokens)
{
	t_parsed_cmd		*parsed_cmd;
	t_parsed_cmd_list	*parsed_cmd_list;
	string_list			*from_token;

	parsed_cmd_list = NULL;
	from_token = tokens;
	parsed_cmd = create_init_parsed_cmd();
	while (tokens != NULL)
	{
		if (tokens->next != NULL)
		{
			if (tokens->next->string[0] == '|' \
					|| tokens->next->string[0] == ';')
			{
				parsed_cmd->is_piped = (tokens->next->string[0] == '|');
				if (parse_fill_parsed_cmd(parsed_cmd, from_token, tokens, \
							tokens->next->string[0]) == -1)
					return (NULL);
				append_new_pcl(&parsed_cmd_list, parsed_cmd);
				tokens = tokens->next;
				if (tokens->next != NULL)
				{
					parsed_cmd = create_init_parsed_cmd();
					from_token = tokens->next;
				}
			}
		}
		else
		{
			parse_fill_parsed_cmd(parsed_cmd, from_token, tokens, ';');
			append_new_pcl(&parsed_cmd_list, parsed_cmd);
			break ;
		}
		tokens = tokens->next;
	}
	return (parsed_cmd_list);
}
