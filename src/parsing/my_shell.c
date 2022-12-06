#include "../../my_shell.h"

void executeCommandList(t_parsed_cmd_managed_list *parsed_cmd_managed_list, char *raw_command_line)
{
    t_parsed_cmd_managed_list *parsed_cmd_managed_list_ = parsed_cmd_managed_list;

    while (parsed_cmd_managed_list_ != NULL)
    {
        // executecommand(parsed_cmd_list->command);
        parsed_cmd_managed_list_ = parsed_cmd_managed_list_->next;
    }
}

int main(int argc, char **argv)
{
    // init env : charger l'environement du Linux dans le tableau d'environement de notre shell
    t_parsed_cmd_managed_list *parsed_cmd_managed_list;

    while (1)
    {
        char *cmdLine;
        cmdLine = readline(getPrompt());
        parsed_cmd_managed_list = launch_parsing_process(cmdLine);
        if (parsed_cmd_managed_list != NULL)
        {
            //executeCommandList(parsed_cmd_managed_list, cmdLine);
        }
    }

    return 0;
}
