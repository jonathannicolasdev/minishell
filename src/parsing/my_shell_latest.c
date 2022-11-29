#include "my_shell.h"


void executeCommandList(t_parsed_cmd_managed_list *parsed_cmd_managed_list, char* raw_command_line){
    t_parsed_cmd_managed_list *parsed_cmd_managed_list_=parsed_cmd_managed_list;
    
    while(parsed_cmd_managed_list_!=NULL){       
        //executecommand(parsed_cmd_list->command);
        parsed_cmd_managed_list_=parsed_cmd_managed_list_->next;
    }
}


int main(int argc, char **argv)
{
    // init env : charger l'environement du Linux dans le tableau d'environement de notre shell
    
    while (1)
    {
        char *cmdLine;
        t_parsed_cmd_list *parsed_cmd_line;
        t_parsed_cmd_managed_list *parsed_cmd_managed_list;
        cmdLine = readline(getPrompt());
        parsed_cmd_line = parse(cmdLine);
        if (parsed_cmd_line!=NULL){
            fill_env_cmd_list(parsed_cmd_line);
            parsed_cmd_managed_list = preprocess(parsed_cmd_line);
            if (parsed_cmd_managed_list!=NULL){
                executeCommandList(parsed_cmd_managed_list,cmdLine);
            }
        }
    }
    return 0;
}
