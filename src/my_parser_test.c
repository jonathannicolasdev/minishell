#include "../my_shell.h"

// redirect_list * append_new_redirect(redirect_list * redir){
//     if (redir->next==NULL){
//         &(redir->next)= malloc
//     }
// }

int is_redirect_token(char *c, int token_size)
{
  if (token_size == 1)
  {
    return *c == '<' || *c == '>' || *c == '|' || *c == ';';
  }
  if (token_size == 2)
  {
    return (*c == '<' || *c == '>') && *c == *++c;
  }
  return 0;
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
    if (cmd->input_direction != NULL)
    {
      redirect_list *ptr_redir = cmd->input_direction;
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
        case PIPE_:
          printf("Redirection d'entree | \n");
          break;
        case SEMICOLON:
          printf("Redirection d'entree SEMICOLON ; \n");
        }
        ptr_redir = ptr_redir->next;
      }
    }
    if (cmd->output_direction != NULL)
    {
      redirect_list *ptr_redir = cmd->output_direction;
      while (ptr_redir != NULL)
      {
        switch (ptr_redir->direction)
        {
        case OUTPUT_FILE_CREATE:
          printf("Redirection de sortie > vers %s\n", ptr_redir->source);
          break;
        case OUTPUT_FILE_APPEND:
          printf("Redirection de sortie >> vers %s\n", ptr_redir->source);
          break;
        case PIPE_:
          printf("Redirection de sortie | \n");
          break;
        case SEMICOLON:
          printf("Redirection de sortie SEMICOLON ; \n");
        }
        ptr_redir = ptr_redir->next;
      }
    }
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
  token->next = recursive_extract_tokens(text + tk_len);

  return token;
}

void init_parsing_struct(t_parsed_cmd_list *command_line)
{
}

t_parsed_cmd_list *parse(char *cmdline)
{
  string_list *tokens;
  string_list *ptr_arg;
  redirect_list *ptr_redi_in;
  redirect_list *ptr_redi_out;

  tokens = recursive_extract_tokens(cmdline);
  print_string_list(tokens);

  // fill_env_tokens(tokens);
  //  print_string_list(tokens);
  t_parsed_cmd_list *parsed_cmd_list = NULL, *first_parsed_cmd_list;
  int first = 1;
  int last_piped = 0;
  int last_semicolon=0;
  t_parsed_cmd *parsed_cmd;
  while (tokens != NULL)
  {
    if (first)
    {
      int no_command = 0;
      parsed_cmd = (t_parsed_cmd *)malloc(sizeof(t_parsed_cmd));
      if (is_redirect_token(tokens->string, 1))
      {
        no_command = 1;
        parsed_cmd->name = strdup("");
      }
      else
        parsed_cmd->name = strdup(tokens->string);
      parsed_cmd->arguments = (string_list *)NULL;
      parsed_cmd->input_direction = (redirect_list *)NULL;
      parsed_cmd->output_direction = (redirect_list *)NULL;

      ptr_arg = NULL;
      ptr_redi_in = NULL;
      ptr_redi_out = NULL;
      if (last_piped)
      {
        ptr_redi_in = (redirect_list *)malloc(sizeof(redirect_list));
        ptr_redi_in->direction = PIPE_;
        ptr_redi_in->next = NULL;
        parsed_cmd->input_direction = ptr_redi_in;
        last_piped = 0;
      }
      if (last_semicolon)
      {
        ptr_redi_in = (redirect_list *)malloc(sizeof(redirect_list));
        ptr_redi_in->direction = SEMICOLON;
        ptr_redi_in->next = NULL;
        parsed_cmd->input_direction = ptr_redi_in;
        last_semicolon = 0;
      }
      
      
      first = 0;

      if (parsed_cmd_list == NULL)
      {
        parsed_cmd_list = (t_parsed_cmd_list *)malloc(sizeof(t_parsed_cmd_list));
        first_parsed_cmd_list = parsed_cmd_list;
      }
      else
      {
        parsed_cmd_list->next = (t_parsed_cmd_list *)malloc(sizeof(t_parsed_cmd_list));
        parsed_cmd_list = parsed_cmd_list->next;
      }
      parsed_cmd_list->command = parsed_cmd;
      parsed_cmd_list->next = NULL;
      if (!no_command)
        tokens = tokens->next;
      continue;
    }
    if (strlen(tokens->string) == 2 && is_redirect_token(tokens->string, 2))
    {
      if (tokens->string[0] == '>')
      {
        if (parsed_cmd->output_direction == NULL)
        {
          ptr_redi_out = (redirect_list *)malloc(sizeof(redirect_list));
          parsed_cmd->output_direction = ptr_redi_out;
        }
        else
        {
          ptr_redi_out->next = (redirect_list *)malloc(sizeof(redirect_list));
          ptr_redi_out = ptr_redi_out->next;
        }
        ptr_redi_out->direction = OUTPUT_FILE_APPEND;
        ptr_redi_out->source = strdup(tokens->next->string);
        ptr_redi_out->next = NULL;
      }
      else
      {
        if (parsed_cmd->input_direction == NULL)
        {
          ptr_redi_in = (redirect_list *)malloc(sizeof(redirect_list));
          parsed_cmd->input_direction = ptr_redi_in;
        }
        else
        {
          ptr_redi_in->next = (redirect_list *)malloc(sizeof(redirect_list));
          ptr_redi_in = ptr_redi_in->next;
        }
        ptr_redi_in->direction = INPUT_NEXT_LINE;
        ptr_redi_in->source = strdup(tokens->next->string);
        ptr_redi_in->next = NULL;
      }
      tokens = tokens->next;
    }
    else
    {
      if (is_redirect_token(tokens->string, 1))
      {
        if (tokens->string[0] == '>')
        {
          if (parsed_cmd->output_direction == NULL)
          {
            ptr_redi_out = (redirect_list *)malloc(sizeof(redirect_list));
            parsed_cmd->output_direction = ptr_redi_out;
          }
          else
          {
            ptr_redi_out->next = (redirect_list *)malloc(sizeof(redirect_list));
            ptr_redi_out = ptr_redi_out->next;
          }
          ptr_redi_out->direction = OUTPUT_FILE_CREATE;
          ptr_redi_out->source = strdup(tokens->next->string);
          ptr_redi_out->next = NULL;
          tokens = tokens->next;
        }
        else if (tokens->string[0] == '<')
        {
          if (parsed_cmd->input_direction == NULL)
          {
            ptr_redi_in = (redirect_list *)malloc(sizeof(redirect_list));
            parsed_cmd->input_direction = ptr_redi_in;
          }
          else
          {
            ptr_redi_in->next = (redirect_list *)malloc(sizeof(redirect_list));
            ptr_redi_in = ptr_redi_in->next;
          }
          ptr_redi_in->direction = INPUT_FILE;
          ptr_redi_in->source = strdup(tokens->next->string);
          ptr_redi_in->next = NULL;
          tokens = tokens->next;
        }
        if (tokens->string[0] == '|')
        {
          if (parsed_cmd->output_direction == NULL)
          {
            ptr_redi_out = (redirect_list *)malloc(sizeof(redirect_list));
            parsed_cmd->output_direction = ptr_redi_out;
          }
          else
          {
            ptr_redi_out->next = (redirect_list *)malloc(sizeof(redirect_list));
            ptr_redi_out = ptr_redi_out->next;
          }
          ptr_redi_out->direction = PIPE_;
          ptr_redi_out->source = NULL;
          ptr_redi_out->next = NULL;
          first = 1;
          last_piped = 1;
          // a pipe
        }
        if (tokens->string[0] == ';')
        {
          if (parsed_cmd->output_direction == NULL)
          {
            ptr_redi_out = (redirect_list *)malloc(sizeof(redirect_list));
            parsed_cmd->output_direction = ptr_redi_out;
          }
          else
          {
            ptr_redi_out->next = (redirect_list *)malloc(sizeof(redirect_list));
            ptr_redi_out = ptr_redi_out->next;
          }
          ptr_redi_out->direction = SEMICOLON;
          ptr_redi_out->source = NULL;
          ptr_redi_out->next = NULL;
          first = 1;
          last_semicolon = 1;
          // a semicolon
        }
      }
      else
      {
        string_list *arg = create_arg_from_token(tokens->string);
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
    }
    tokens = tokens->next;
  }
  // t_parsed_cmd_list* parsed_cmd_list=malloc(sizeof(parsed_cmd_list));
  // parsed_cmd_list->command=parsed_cmd;
  // parsed_cmd_list->next=NULL;

  // print_parsing_struct(first_parsed_cmd_list);
  return first_parsed_cmd_list;
}

void free_parsing_struct(t_parsed_cmd_list *command_line)
{
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
          perror("incorrect envirement reference format");
         // return;
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
        return newTokenVal;
      }
      else
        return token;
    }
  }
  else
    return token;
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
  if (cmd->input_direction != NULL)
  {
    redirect_list *ptr_redir = cmd->input_direction;
    while (ptr_redir != NULL)
    {
      switch (ptr_redir->direction)
      {
      case INPUT_FILE:
      case INPUT_NEXT_LINE:
        ptr_redir->source = fill_env_token(ptr_redir->source);
      }
      ptr_redir = ptr_redir->next;
    }
  }
  if (cmd->output_direction != NULL)
  {
    redirect_list *ptr_redir = cmd->output_direction;
    while (ptr_redir != NULL)
    {
      switch (ptr_redir->direction)
      {
      case OUTPUT_FILE_CREATE:
      case OUTPUT_FILE_APPEND:
        ptr_redir->source = fill_env_token(ptr_redir->source);
      }
      ptr_redir = ptr_redir->next;
    }
  }
}
