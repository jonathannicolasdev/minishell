#include "my_shell.h"

int isBuiltInCommand(t_parsed_cmd *command){
    return 1;
}


int executeCommand(t_parsed_cmd *cmd){
    return 1;
}

int executeBuiltInCommand(t_parsed_cmd *cmd){
    return 1;
}


int main(int argc, char **argv)
{
    while (1)
    {
        int childPid;
        char *cmdLine;
        t_parsed_cmd_list *cmd_line;
        t_parsed_cmd *cmd = NULL;

        cmdLine = readline(getPrompt());
        cmd_line = parse(cmdLine);

        // record command in history list (GNU readline history ?)

        if (isBuiltInCommand(cmd))
        {
            executeBuiltInCommand(cmd);
        }
        else
        {
            childPid = fork();
            if (childPid == 0)
            {
                executeCommand(cmd); // calls execvp
            }
        }
    }
    return 0;
}
