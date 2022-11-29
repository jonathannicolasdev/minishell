#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <fcntl.h>

typedef enum
{
    INPUT_FILE,         // redirection < fichier_input.txt
    INPUT_NEXT_LINE,    //<<
    OUTPUT_FILE_CREATE, // > fichier_output.txt
    OUTPUT_FILE_APPEND, // >> fichier_output.txt
    PIPE_ ,             // |
    SEMICOLON,          // ;
} t_io_direction;

typedef enum
{
    BUILTIN,
    EXTERNAL
} t_command_type;

typedef struct string_list_
{
    char *string;
    struct string_list_ *next;
} string_list;

typedef struct redirect_list_
{
    t_io_direction direction;
    char *source;
    struct redirect_list_ *next;
} redirect_list;

typedef struct t_parsed_cmd_ {
    t_command_type type;
    char *name;    
    string_list *arguments;
    redirect_list *input_direction;
    redirect_list *output_direction;
} t_parsed_cmd;

typedef struct {
    t_command_type type;
    char *name;
    char *path;    
    char **argv;
    int argc;
    int in_desc;
    int out_desc;
    int is_piped;
} t_parsed_cmd_managed;

typedef struct t_parsed_cmd_managed_list_
{
    t_parsed_cmd_managed *command;
    struct t_parsed_cmd_managed_list_ *next;

} t_parsed_cmd_managed_list;

typedef struct t_command_line_
{
    t_parsed_cmd *command;
    struct t_command_line_ *next;

} t_parsed_cmd_list;

char *getPrompt();
t_parsed_cmd_list *parse(char *);
void print_string_list(string_list *list);
void print_parsing_struct(t_parsed_cmd_list *command_line);
void fill_env_cmd(t_parsed_cmd *cmd);
char* get_my_env(char* env_var); // sachant que la table d'environement est une donnee globale
void set_my_env(char* env_var, char* env_val); // pareil
t_parsed_cmd_managed_list *preprocess(t_parsed_cmd_list *command_line);



