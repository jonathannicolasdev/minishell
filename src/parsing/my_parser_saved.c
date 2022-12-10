#include "../../my_shell.h"

redirect_list *append_new_redirect(redirect_list **redir, char *source, t_io_direction direction)
{
  redirect_list *cell = (redirect_list *)malloc(sizeof(redirect_list));
  redirect_list *ptr;
  cell->source = source;
  cell->direction = direction;
  cell->next = NULL;
  if (*redir == NULL)
  {
    *redir = cell;
    return cell;
  }
  ptr = *redir;
  while (ptr->next != NULL)
  {
    ptr = ptr->next;
  }
  ptr->next = cell;
  return cell;
}

t_parsed_cmd_list *append_new_pcl(t_parsed_cmd_list **parsed_cmd_list, t_parsed_cmd *parsed_cmd)
{
  t_parsed_cmd_list *cell = (t_parsed_cmd_list *)malloc(sizeof(t_parsed_cmd_list));
  t_parsed_cmd_list *ptr;

  cell->command = parsed_cmd;
  cell->next = NULL;
  if (*parsed_cmd_list == NULL)
  {
    *parsed_cmd_list = cell;
    return cell;
  }
  ptr = *parsed_cmd_list;
  while (ptr->next != NULL)
  {
    ptr = ptr->next;
  }
  ptr->next = cell;
  return cell;
}

t_parsed_cmd *create_init_parsed_cmd()
{
  t_parsed_cmd *parsed_cmd;
  parsed_cmd = (t_parsed_cmd *)malloc(sizeof(t_parsed_cmd));
  parsed_cmd->name = (char *)NULL;
  parsed_cmd->arguments = (string_list *)NULL;
  parsed_cmd->redirections = (redirect_list *)NULL;
  parsed_cmd->is_piped = 0;

  return parsed_cmd;
}

int is_redirect_token(char *c, int token_size)
{
  if (token_size == 1)
  {
    return *c == '<' || *c == '>' || *c == '|' || *c == ';';
  }
  if (token_size == 2)
  {
    if (strlen(c) != 2)
      return 0;
    return (*c == '<' || *c == '>') && *c == *++c;
  }
  return 0;
}

int redirect_token_type(char *c)
{
  if (strlen(c) == 1)
  {
    switch (*c)
    {
    case '<':
      return INPUT_FILE;
    case '>':
      return OUTPUT_FILE_CREATE;
    }
  }
  if (strlen(c) == 2)
  {
    if ((*c == '<') && *(c + 1) == '<')
      return INPUT_NEXT_LINE;
    if ((*c == '>') && *(c + 1) == '>')
      return OUTPUT_FILE_APPEND;
  }
  return NO_REDIR;
}

int correct_envvar_char(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

int redirect_position(char *str, int len)
{
  for (int i = 0; i < len; i++)
  {
    if (str[i] == '<' || str[i] == '>' || str[i] == '|' || str[i] == ';')
      return i;
  }
  return -1;
}

string_list *create_token(char *value)
{
  string_list *token;
  token = (string_list *)malloc(sizeof(string_list));
  token->string = value;
  return token;
}

string_list *create_arg_from_token(char *value)
{
  string_list *arg;
  arg = (string_list *)malloc(sizeof(string_list));
  arg->string = strdup(value);
  arg->next = NULL;
  return arg;
}

void print_parsing_struct(t_parsed_cmd_list *command_line)
{
  t_parsed_cmd *cmd;
  while (command_line != NULL)
  {
    cmd = command_line->command;

    printf("execution de la commande: %s\n", cmd->name);
    if (cmd->arguments)
    {
      printf("avec les arguments: ");
      string_list *arglist = cmd->arguments;
      while (arglist != NULL)
      {
        printf("[%s] ", arglist->string);
        arglist = arglist->next;
      }
      printf("\n");
    }
    if (cmd->redirections != NULL)
    {
      redirect_list *ptr_redir = cmd->redirections;
      while (ptr_redir != NULL)
      {
        switch (ptr_redir->direction)
        {
        case INPUT_FILE:
          printf("Redirection d'entree < depuis %s\n", ptr_redir->source);
          break;
        case INPUT_NEXT_LINE:
          printf("Redirection d'entree << depuis %s\n", ptr_redir->source);
          break;
        case OUTPUT_FILE_CREATE:
          printf("Redirection de sortie > vers %s\n", ptr_redir->source);
          break;
        case OUTPUT_FILE_APPEND:
          printf("Redirection de sortie >> vers %s\n", ptr_redir->source);
        }
        ptr_redir = ptr_redir->next;
      }
    }
    if (cmd->is_piped)
      printf("Redirection de sortie de type PIPE |\n");
    command_line = command_line->next;
  }
}

string_list *recursive_extract_tokens(char *text)
{
  string_list *token;
  char stopChar;
  char *tokenValue;
  int quoted;
  int tk_len = 0;

  while (*text == ' ')
    text++;
  if (!*text)
    return NULL;
  if (*text == '\'' || *text == '"')
  {
    quoted = 1;
    stopChar = *text;
  }
  else
    quoted = 0;
  if (quoted)
  {
    tk_len = 1;
    while (text[tk_len] != stopChar && text[tk_len])
      tk_len++;
    tk_len++;
  }
  else
  {
    tk_len = 0;
    while (text[tk_len] != ' ' && text[tk_len])
      tk_len++;
    int posredir = redirect_position(text, tk_len);
    if (posredir != -1)
    {
      int redir_len = 1;
      if (text[posredir] == text[posredir + 1])
        redir_len = 2;
      if (posredir == 0)
        tk_len = redir_len;
      else
        tk_len = posredir;
    }
  }
  tokenValue = (char *)malloc(sizeof(char) * (tk_len + 1));
  strncpy(tokenValue, text, tk_len);
  tokenValue[tk_len] = '\0';
  token = create_token(tokenValue);
  printf("%s\n", token->string);
  token->next = recursive_extract_tokens(text + tk_len);

  return token;
}

int parse_fill_parsed_cmd(t_parsed_cmd *parsed_cmd, string_list *from_token, string_list *to_token, char separator)
{
  string_list *tokens;
  string_list *ptr_arg;
  redirect_list *ptr_redir;
  // printf("parse_fill from token[@%x] to token[@%x]\n", from_token, to_token);

  if (!is_redirect_token(from_token->string, 1))
  {
    parsed_cmd->name = strdup(from_token->string);
    from_token = from_token->next;
  }
  ptr_arg = NULL;
  ptr_redir = NULL;
  while (from_token != to_token)
  {
    if (redirect_token_type(from_token->string) != NO_REDIR)
    {
      if (from_token->next == NULL)
      {
        perror("syntax error near unexpected token `newline'");
        return -1;
      }
      if (redirect_token_type(from_token->next->string) != NO_REDIR)
      {
        printf("syntax error near unexpected token `%c'", from_token->next->string[0]);
        return -1;
      }

      ptr_redir = append_new_redirect(&(parsed_cmd->redirections), strdup(from_token->next->string), redirect_token_type(from_token->string));
      from_token = from_token->next;
      if (from_token == to_token)
        break;
    }
    else
    {
      string_list *arg = create_arg_from_token(from_token->string);
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
    from_token = from_token->next;
  }
  return 1;
}

t_parsed_cmd_list *create_parsed_cmd_list(string_list *tokens)
{
  t_parsed_cmd *parsed_cmd;
  t_parsed_cmd_list *parsed_cmd_list = NULL;
  string_list *from_token;

  from_token = tokens;
  parsed_cmd = create_init_parsed_cmd();
  while (tokens != NULL)
  {
    if (tokens->next != NULL)
    {
      if (tokens->next->string[0] == '|' || tokens->next->string[0] == ';')
      {
        parsed_cmd->is_piped = (tokens->next->string[0] == '|');
        if (parse_fill_parsed_cmd(parsed_cmd, from_token, tokens, tokens->next->string[0]) == -1)
          return NULL;
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
      break;
    }
    tokens = tokens->next;
  }
  return parsed_cmd_list;
}

void fill_env_cmd_list(t_parsed_cmd_list *parsed_cmd_list)
{
  while (parsed_cmd_list != NULL)
  {
    fill_env_cmd(parsed_cmd_list->command);
    parsed_cmd_list = parsed_cmd_list->next;
  }
}

void free_parsing_struct(t_parsed_cmd_list *command_line)
{
}

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

char *fill_env_token(char *token)
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
        // really we must stop on next char not allowed for var name, and not just spaces
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

void fill_env_cmd(t_parsed_cmd *cmd)
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

t_parsed_cmd_managed_list *launch_parsing_process(char *cmdLine)
{
  t_parsed_cmd_list *parsed_cmd_list;
  t_parsed_cmd_managed_list *parsed_cmd_managed_list;
  string_list *tokens;

  // 1- extraction des tokens
  tokens = recursive_extract_tokens(cmdLine);
  if (tokens == NULL)
    return NULL;
  print_string_list(tokens);

  // 2- parsing des tokens et generation de la liste des commandes
  parsed_cmd_list = create_parsed_cmd_list(tokens);

  if (parsed_cmd_list != NULL)
  {
    // 3- Lecture des variable d'environement
    fill_env_cmd_list(parsed_cmd_list);

    print_parsing_struct(parsed_cmd_list);

    // 4- ouverture des fichiers de redirection et creation des structure de commande pretes a l'execution
    parsed_cmd_managed_list = preprocess(parsed_cmd_list);
    printf("%p\n", parsed_cmd_managed_list);
    print_managed_parsing_struct(parsed_cmd_managed_list);
    return parsed_cmd_managed_list;
  }
  else
    return NULL;
}
