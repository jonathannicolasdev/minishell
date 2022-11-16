#include "../my_shell.h"
/*
string_list *scanTokens(char *commandText)
{
  return NULL;
}

void init_parsing_struct(t_parsed_cmd_list *command_line)
{
}
*/
string_list* extract_tokens(char *text){
  string_list* tokens_start;
  string_list* tokens;
  int tk_start=0, tk_end=0;

  tokens_start = (string_list*)malloc(sizeof(string_list));
  tokens_start->string=NULL;
  tokens=tokens_start;

  while (text[tk_start]){
    while (text[tk_start]==' ') tk_start++;
    if (!text[tk_start]){
      break;
    }
    tk_end=tk_start;
    while (text[tk_end]!=' ' && text[tk_end]) tk_end++;
    
    if (tokens->string!=NULL){
      tokens->next= (string_list*)malloc(sizeof(string_list));
      tokens=tokens->next;   
    }
    tokens->string=(char *)malloc(sizeof(char)*(tk_end-tk_start));
    strncpy(tokens->string,&text[tk_start],tk_end-tk_start);
    tokens->string[tk_end-tk_start]='\0';
    tokens->next=NULL;
    tk_start=tk_end;   
  }

  return tokens_start;
}

t_parsed_cmd_list *parse(char *cmdline)
{ 
  string_list* tokens;

  tokens = extract_tokens(cmdline);
  print_string_list(tokens);
  return NULL;
}
/*
void print_parsing_struct(t_parsed_cmd_list *command_line)
{
}

void free_parsing_struct(t_parsed_cmd_list *command_line)
{
}
*/

void test(char *commandText)
{
  t_parsed_cmd_list *parsed_cmd_list;

  parsed_cmd_list = parse(commandText);
 // print_parsing_struct(parsed_cmd_list);
 // free_parsing_struct(parsed_cmd_list);
}
