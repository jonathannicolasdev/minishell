#include "../my_shell.h"


void executeCommandList(t_parsed_cmd_list *parsed_cmd_list, char* raw_command_line){
    t_parsed_cmd_list *parsed_cmd_list_=parsed_cmd_list;
    
    while(parsed_cmd_list!=NULL){
        fill_env_cmd(parsed_cmd_list->command);
        //executecommand(parsed_cmd_list->command);
        parsed_cmd_list=parsed_cmd_list->next;
    }
    print_parsing_struct(parsed_cmd_list_);
}


int main(int argc, char **argv)
{
    while (1)
    {
        char *cmdLine;
        t_parsed_cmd_list *parsed_cmd_line;

        cmdLine = readline(getPrompt());
        parsed_cmd_line = parse(cmdLine);
        executeCommandList(parsed_cmd_line,cmdLine);
    }
    return 0;
}
