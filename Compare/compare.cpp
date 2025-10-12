#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "compare.h"

int main()
{
    FILE *original_commands = fopen("original_commands.asm", "r");
    FILE *received_commands = fopen("received_commands.asm", "r");

    if (!open_files_success(original_commands, received_commands))
        return -1;

    struct commands_parameters commands_from_files =
    {
        .command1 = {0},
        .command2 = {0},
        .argument1 = 0,
        .argument2 = 0,
        .num_of_strings = 1
    };
    
    while (!feof(original_commands) && !feof(received_commands))
    {
        if (fscanf(original_commands, "%s", commands_from_files.command1) != 1)
        {
            printf("\033[31min original_commands.asm: line %zd: The command is not set.\033[0m\n", commands_from_files.num_of_strings);
            return 1;
        }
        else if (fscanf(received_commands, "%s", commands_from_files.command2) != 1)
        {
            printf("\033[31min received_commands.asm: line %zd: The command is not set.\033[0m\n", commands_from_files.num_of_strings);
            return 1;
        };

        if (strcmp(commands_from_files.command1, commands_from_files.command2))
        {
            printf("\033[31min original_commands.asm and received_commands.asm line %zd doesn't match: %s and %s\033[0m\n", commands_from_files.num_of_strings, commands_from_files.command1, commands_from_files.command2);
            return 2;
        }

        if (!strcmp(commands_from_files.command1, "PUSH"))
        {
            if (fscanf(original_commands, "%d", &commands_from_files.argument1) != 1)
            {
                printf("\033[31min original_commands.asm: line %zd: The PUSH command argument is missing.\033[0m\n", commands_from_files.num_of_strings);
                return 3;
            }

            if (fscanf(received_commands, "%d", &commands_from_files.argument2) != 1)
            {
                printf("\033[31min received_commands.asm: line %zd: The PUSH command argument is missing.\033[0m\n", commands_from_files.num_of_strings);
                return 3;
            }
            
            if (commands_from_files.argument1 != commands_from_files.argument2)
            {
                printf("\033[31min original_commands.asm and received_commands.asm: line %zd: The arguments of the PUSH command are different.\033[0m\n", commands_from_files.num_of_strings);
                return 4;
            }
        }

        commands_from_files.num_of_strings++;
    }

    return 0;
}

bool open_files_success(FILE *original_commands, FILE *received_commands)
{
    if (original_commands == NULL)
    {
        printf("An error occurred when opening the file \"original_commands.asm\"");
        return false;
    }
    if (received_commands == NULL)
    {
        printf("An error occurred when opening the file \"received_commands.asm\"");
        return false;
    }
    return true;
}