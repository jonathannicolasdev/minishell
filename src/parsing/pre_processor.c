#include "../../my_shell.h"

void print_managed_parsing_struct(t_parsed_cmd_managed_list *parsed_cmd_managed_list)
{
    t_parsed_cmd_managed *cmd;
    while (parsed_cmd_managed_list != NULL)
    {
        cmd = parsed_cmd_managed_list->command;

        printf("execution de la commande: %s\n", cmd->name);
        for (int i = 0; i < cmd->argc; i++)
            printf("[%s] ", cmd->argv[i]);
        printf("\n");
        printf("Redirection d'entree sur le descripteur %d\n", cmd->in_desc);
        printf("Redirection de sortie sur le descripteur %d\n", cmd->out_desc);
        if (cmd->is_piped)
            printf("Redirection de sortie de type PIPE |\n");
        parsed_cmd_managed_list = parsed_cmd_managed_list->next;
    }
}

t_parsed_cmd_managed_list *append_new_cmd(t_parsed_cmd_managed_list **firstcell)
{
    t_parsed_cmd_managed_list *cell = (t_parsed_cmd_managed_list *)malloc(sizeof(t_parsed_cmd_managed_list));
    t_parsed_cmd_managed_list *ptr;
    cell->next = NULL;
    if (*firstcell == NULL)
    {
        *firstcell = cell;
        return cell;
    }
    ptr = *firstcell;
    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }
    ptr->next = cell;
    return cell;
}

char **create_argv(string_list *arguments, int *length)
{
    char **argv;
    int len = 0;
    string_list *arguments_ = arguments;
    while (arguments != NULL)
    {
        arguments = arguments->next;
        len++;
    }
    *length = len;
    if (len == 0)
        return NULL;

    argv = malloc(len * sizeof(char *));
    arguments = arguments_;
    int i = 0;
    while (arguments != NULL)
    {
        argv[i++] = strdup(arguments->string);
        arguments = arguments->next;
    }
    return argv;
}

// return -2 if error opening a file in write/append mode
// return -1 if no file to open
int *open_input_ouput_files(redirect_list *redirections)
{
    int *fd = malloc(2 * sizeof(int));
    fd[0] = -1; // for input descriptor
    fd[1] = -1; // for output descriptor

    while (redirections != NULL)
    {
        if (redirections->direction == OUTPUT_FILE_CREATE)
        {
            fd[1] = open(redirections->source, O_CREAT | O_WRONLY, 0644);
            if (fd[1] == -1)
            {
                printf("failed to open file %s in write mode\n", redirections->source);
                fd[1] = -2;
                return fd;
            }
        }
        if (redirections->direction == OUTPUT_FILE_APPEND)
        {
            fd[1] = open(redirections->source, O_CREAT | O_APPEND, 0644);
            if (fd[1] == -1)
            {
                printf("failed to open file %s in append mode\n", redirections->source);
                fd[1] = -2;
                return fd;
            }
        }
        if (redirections->direction == INPUT_FILE)
        {
            fd[0] = open(redirections->source, O_RDONLY);
            if (fd[0] == -1)
            {
                printf("failed to open file %s in read mode\n", redirections->source);
                fd[0] = -2;
                return fd;
            }
        }

        // le traitement du cas << est a ajouter ici

        redirections = redirections->next;
    }
    return fd;
}

t_parsed_cmd_managed_list *preprocess(t_parsed_cmd_list *command_line)
{
    t_parsed_cmd_managed_list *parsed_cmd_managed_list = NULL, *ptr;
    int *fd;
    t_parsed_cmd *cmd;
    while (command_line != NULL)
    {lg();
        cmd = command_line->command;
        ptr = append_new_cmd(&parsed_cmd_managed_list);
        ptr->command = malloc(sizeof(t_parsed_cmd_managed));
        ptr->command->name = strdup(cmd->name);
        fd = open_input_ouput_files(cmd->redirections);
        if (fd[0] == -2 || fd[1] == -2)
            return NULL;
        ptr->command->in_desc = fd[0];
        ptr->command->out_desc = fd[1];
        ptr->command->argv = create_argv(cmd->arguments, &ptr->command->argc);
        ptr->command->is_piped = cmd->is_piped;
        command_line = command_line->next;
    }
    free(fd);
    return parsed_cmd_managed_list;
}
