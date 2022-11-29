#include "my_shell.h"

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

int is_piped(redirect_list *output_direction)
{
    while (output_direction != NULL)
    {
        if (output_direction->source == PIPE_)
            return 1;
        output_direction = output_direction->next;
    }
    return 0;
}

// return -1 if error opening a file in read mode
// return -2 if no file to open
int open_inputfiles(redirect_list *input_direction)
{
    int fd = -2;

    while (input_direction != NULL)
    {
        if (input_direction->direction == INPUT_FILE)
        {
            fd = open(input_direction->source, O_RDONLY);
            if (fd == -1)
                return -1;
        }
        input_direction = input_direction->next;
    }
    return fd;
}

// return -1 if error opening a file in write/append mode
// return -2 if no file to open
int open_ouputfiles(redirect_list *output_direction)
{
    int fd = -2;

    while (output_direction != NULL)
    {
        if (output_direction->direction == OUTPUT_FILE_CREATE)
        {
            fd = open(output_direction->source, O_WRONLY);
            if (fd == -1)
                return -1;
        }
        if (output_direction->direction == OUTPUT_FILE_APPEND)
        {
            fd = open(output_direction->source, O_APPEND);
            if (fd == -1)
                return -1;
        }

        output_direction = output_direction->next;
    }
    return fd;
}

t_parsed_cmd_managed_list *preprocess(t_parsed_cmd_list *command_line)
{
    t_parsed_cmd_managed_list *parsed_cmd_managed_list = NULL, *ptr;
    int res;
    t_parsed_cmd *cmd;
    while (command_line != NULL)
    {
        print_parsing_struct(command_line);

        cmd = command_line->command;
        ptr = append_new_cmd(parsed_cmd_managed_list);
        ptr->command = malloc(sizeof(t_parsed_cmd_managed));
        ptr->command->name = strdup(cmd->name);
        res = open_inputfiles(cmd->input_direction);
        if (res == -1)
        {
            perror("failed to open file in read mode");
            return NULL;
        }
        if (res >= 0)
            ptr->command->in_desc = res;
        else
            ptr->command->in_desc = -1; // -1 no file redirection, standard

        res = open_ouputfiles(cmd->output_direction);
        if (res == -1)
        {
            perror("failed to open file in write mode");
            return NULL;
        }
        if (res >= 0)
            ptr->command->out_desc = res;
        else
            ptr->command->out_desc = -1; // -1 no file redirection, standard

        ptr->command->argv = create_argv(cmd->arguments, &ptr->command->argc);

        ptr->command->is_piped = is_piped(cmd->output_direction);

        command_line = command_line->next;
    }
}
