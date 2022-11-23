#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

typedef enum
{
    STANDARD_,          // normal
    INPUT_FILE,         // redirection < fichier_input.txt
    INPUT_NEXT_LINE,    //<<
    OUTPUT_FILE_CREATE, // > fichier_output.txt
    OUTPUT_FILE_APPEND, // >> fichier_output.txt
    PIPE_               // |
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

typedef struct
{
    t_command_type type;
    char *name;
    string_list *arguments;
    t_io_direction input_direction;
    t_io_direction output_direction;
    char *input_source; // file or scan until stop (<<)
    char *output_file;
} t_parsed_cmd;

typedef struct t_command_line_
{
    t_parsed_cmd *command;
    struct t_parsed_cmd_list *next;

} t_parsed_cmd_list;

char *getPrompt();
t_parsed_cmd_list *parse(char *);
void print_string_list(string_list *list);
void lg();
