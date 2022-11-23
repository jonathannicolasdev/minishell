#include "../my_shell.h"

int is_redirect_token(char *c, int token_size)
{
  if (token_size == 1)
  {
    return *c == '<' || *c == '>' || *c == '|';
  }
  if (token_size == 2)
  {
    return (*c == '<' || *c == '>') && *c == *++c;
  }
  return 0;
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
        printf("%s ", arglist->string);
        arglist = arglist->next;
      }
      printf("\n");
      if (cmd->input_direction != STANDARD_)
      {
        int type_red = 1;
        if (cmd->input_direction == INPUT_NEXT_LINE)
          type_red = 2;
        if (type_red == 1)
          printf("Redirection d'entree < depuis %s\n", cmd->input_source);
        else
          printf("Redirection d'entree << depuis %s\n", cmd->input_source);
      }
      if (cmd->output_direction != STANDARD_)
      {
        int type_red = 1;
        if (cmd->output_direction == OUTPUT_FILE_APPEND)
          type_red = 2;
        if (type_red == 1)
          printf("Redirection de sortie > vers %s\n", cmd->output_file);
        else
          printf("Redirection d'entree << depuis %s\n", cmd->output_file);
      }
    }
    command_line = command_line->next;
  }
}

string_list *recursive_extract_tokens(char *text)
{
  string_list *token;
  int tk_start = 0, tk_end = 0;
  char stopChar;
  char *tokenValue;
  int quoted;

  while (text[tk_start] == ' ')
    tk_start++;
  if (!text[tk_start])
  {
    return NULL;
  }
  tk_end = tk_start;
  if (text[tk_start] == '\'' || text[tk_start] == '"')
  {
    quoted = 1;
    stopChar = text[tk_start];
  }
  else
    quoted = 0;

  if (quoted)
  {
    tk_end++;
    while (text[tk_end] != stopChar && text[tk_end])
      tk_end++;
  }
  else
  {
    while (text[tk_end] != ' ' && text[tk_end])
    {
      if (text[tk_end + 1])
      {
        if (is_redirect_token(text + tk_end, 2))
        {
          if (!tk_end > tk_start)
            tk_end += 2;
          break;
        }
      }
      if (is_redirect_token(text + tk_end, 1))
      {
        if (!tk_end > tk_start)
          tk_end++;
        break;
      }
      tk_end++;
    }
    tk_end--;
  }

  tokenValue = (char *)malloc(sizeof(char) * (tk_end - tk_start + 2));
  strncpy(tokenValue, text + tk_start, tk_end - tk_start + 1);
  tokenValue[tk_end - tk_start + 1] = '\0';
  token = create_token(tokenValue);
  // printf("recurs to index %d\n",tk_end+1);
  token->next = recursive_extract_tokens(text + tk_end + 1);

  return token;
}
void fill_env_tokens(string_list *tokens)
{
  while (tokens != NULL)
  {
    if (tokens->string[0] != '\'')
    {
      int i = 0;
      while (tokens->string[i] != '$' && tokens->string[i])
        i++;
      if (tokens->string[i])
      {
        int j = i + 1;
        while (tokens->string[j] != ' ' && tokens->string[j])
          j++;
        j = j - 1;
        if (i == j)
        {
          perror("incorrect envirement reference format");
          return;
        }
        i++;
        int k = 0;
        char *envvar = malloc(sizeof(char) * (j - i + 2));
        int envStart = i - 1;
        int envEnd = j;
        while (i <= j)
          envvar[k++] = tokens->string[i++];
        envvar[k] = '\0';
        char *envval = getenv(envvar);
        int newTokenLen = strlen(tokens->string) - strlen(envvar) - 1 + strlen(envval);
        char *newTokenVal = (char *)malloc(sizeof(char) * newTokenLen);
        newTokenVal[0] = '\0';
        strncpy(newTokenVal, tokens->string, envStart + 1);
        newTokenVal[envStart] = '\0';
        strcat(newTokenVal, envval);
        if (tokens->string[envEnd + 1])
          strcat(newTokenVal, tokens->string[envEnd + 1]);
        free(tokens->string);
        tokens->string = newTokenVal;
      }
    }
    tokens = tokens->next;
  }
}
void init_parsing_struct(t_parsed_cmd_list *command_line)
{
}

t_parsed_cmd_list *parse(char *cmdline)
{
  string_list *tokens;
  string_list *ptr_arg;
  tokens = recursive_extract_tokens(cmdline);
  fill_env_tokens(tokens);
  print_string_list(tokens);
  int first = 1;
  t_parsed_cmd *parsed_cmd;
  while (tokens != NULL)
  {
    if (first)
    {
      parsed_cmd = (t_parsed_cmd *)malloc(sizeof(t_parsed_cmd));
      parsed_cmd->name = strdup(tokens->string);
      parsed_cmd->arguments = (t_parsed_cmd *)NULL;
      ptr_arg = NULL;
      parsed_cmd->output_direction = STANDARD_;
      parsed_cmd->input_direction = STANDARD_;
      first = 0;
      tokens = tokens->next;
      continue;
    }
    if (strlen(tokens->string) == 2 && is_redirect_token(tokens->string, 2))
    {
      if (tokens->string[0] == '>')
      {
        parsed_cmd->output_direction = OUTPUT_FILE_APPEND;
        parsed_cmd->output_file = strdup(tokens->next->string);
      }
      else
      {
        parsed_cmd->input_direction = INPUT_NEXT_LINE;
        parsed_cmd->input_source = strdup(tokens->next->string);
      }
      tokens = tokens->next;
    }
    else
    {
      if (is_redirect_token(tokens->string, 1))
      {
        if (tokens->string[0] == '>')
        {
          parsed_cmd->output_direction = OUTPUT_FILE_CREATE;
          parsed_cmd->output_file = strdup(tokens->next->string);
        }
        else if (tokens->string[0] == '<')
        {
          parsed_cmd->input_direction = INPUT_FILE;
          parsed_cmd->input_source = strdup(tokens->next->string);
        }
        else
        {
          // a pipe
        }
        tokens = tokens->next;
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
        lg();
      }
      tokens = tokens->next;
    }
  }
  t_parsed_cmd_list *parsed_cmd_list = malloc(sizeof(parsed_cmd_list));
  parsed_cmd_list->command = parsed_cmd;
  parsed_cmd_list->next = NULL;
  print_parsing_struct(parsed_cmd_list);
  return parsed_cmd_list;
}

void free_parsing_struct(t_parsed_cmd_list *command_line)
{
}

void test(char *commandText)
{
  t_parsed_cmd_list *parsed_cmd_list;

  parsed_cmd_list = parse(commandText);
  print_parsing_struct(parsed_cmd_list);
  free_parsing_struct(parsed_cmd_list);
}
