#include "../my_shell.h"

int is_redirect_token(char* c, int token_size){
    if (token_size==1){
      return *c=='<' || *c=='>' || *c=='|';
    }
    if (token_size==2){
      return (*c=='<' || *c=='>')&& *c==*++c;
    } 
    return 0;    
}

string_list* create_token(char *value){
  string_list* token;
  token = (string_list*)malloc(sizeof(string_list));
  token->string=value;
  return token;
}

string_list* recursive_extract_tokens(char *text){
  string_list* token;
  int tk_start=0, tk_end=0;
  char stopChar;
  char *tokenValue;
  int quoted;

  while (text[tk_start]==' ') tk_start++;
    if (!text[tk_start]){
      return NULL;
    }
    tk_end=tk_start;
    if (text[tk_start]=='\'' || text[tk_start]=='"')
    {
        quoted=1;
        stopChar=text[tk_start];
    }
    else quoted=0;

    if (quoted){
      while (text[tk_end]!=stopChar && text[tk_end]) tk_end++;
    }
    else {
      while (text[tk_end]!=' ' && text[tk_end]) {
        if (text[tk_end+1]){
          if (is_redirect_token(text+tk_end,2)){
            if (!tk_end>tk_start)
              tk_end+=2;
            break;
          }
        }
        if (is_redirect_token(text+tk_end,1)){
            if (!tk_end>tk_start)
              tk_end++;
            break;
          }
        tk_end++;
      } 
      tk_end--;     
    }

    tokenValue=(char *)malloc(sizeof(char)*(tk_end-tk_start+2));
    strncpy(tokenValue,text+tk_start,tk_end-tk_start+1);
    tokenValue[tk_end-tk_start+1]='\0';
    token= create_token(tokenValue);  
    //printf("recurs to index %d\n",tk_end+1);
    token->next = recursive_extract_tokens(text+tk_end+1);

  return token;
}

//ancienne version non recursive qui traite uniquement les espaces
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


void init_parsing_struct(t_parsed_cmd_list *command_line)
{
}


t_parsed_cmd_list *parse(char *cmdline)
{ 
  string_list* tokens;

  tokens = recursive_extract_tokens(cmdline);
  print_string_list(tokens);
  return NULL;
}

void print_parsing_struct(t_parsed_cmd_list *command_line)
{
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
