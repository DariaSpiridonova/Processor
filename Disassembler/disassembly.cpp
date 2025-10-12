#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "disassembly.h"

int main()
{
    FILE *commands_for_reading = fopen("byte_code.txt", "r");
    FILE *commands_for_record = fopen("received_commands.asm", "w");

    struct commands_for_change data_remember = 
    {
        .command = {0},
        .num_of_command = NON_EXISTING_COMMAND,
        .argument = 0,
        .num_of_parameters = 0,
        .num_of_strings = 1
    };

    fscanf(commands_for_reading, "%d", data_remember.num_of_parameters);
    while (!feof(commands_for_reading))
    {
        
        if (fscanf(commands_for_reading, "%d ", (int *)&data_remember.num_of_command) != 1)
        {
            printf("\033[31min commands.asm: line %zd: the command number has not been received.\033[0m\n", data_remember.num_of_strings);
            return 1;
        }

        else if (!identify_the_command(&data_remember))
        {
            printf("\033[31min commands.asm: line %zd: The command with the number %d does not exist.\033[0m\n", data_remember.num_of_strings, data_remember.num_of_command);
            return 2;
        }

        fprintf(commands_for_record, "%s ", data_remember.command);

        if (!strcmp(data_remember.command, "PUSH"))
        {
            if (fscanf(commands_for_reading, "%d", &data_remember.argument) != 1)
            {
                printf("\033[31min commands.asm: line %zd: The PUSH command argument is missing.\033[0m\n", data_remember.num_of_strings);
                return 3;
            }
            fprintf(commands_for_record, "%d", data_remember.argument);
        }
        fprintf(commands_for_record, "\n");
        data_remember.num_of_strings++;
    }

    return 0;
}

bool identify_the_command(struct commands_for_change *data_remember)
{
    bool command_exist = false;

    if      (is_command(data_remember->num_of_command, PUSH, &command_exist)) data_remember->command = "PUSH";
    else if (is_command(data_remember->num_of_command, ADD, &command_exist)) data_remember->command = "ADD";
    else if (is_command(data_remember->num_of_command, SUB, &command_exist)) data_remember->command = "SUB";
    else if (is_command(data_remember->num_of_command, MUL, &command_exist)) data_remember->command = "MUL";
    else if (is_command(data_remember->num_of_command, DIV, &command_exist)) data_remember->command = "DIV";
    else if (is_command(data_remember->num_of_command, SQRT, &command_exist)) data_remember->command = "SQRT";
    else if (is_command(data_remember->num_of_command, OUT, &command_exist)) data_remember->command = "OUT";
    else if (is_command(data_remember->num_of_command, HLT, &command_exist)) data_remember->command = "HLT"; 

    return command_exist;
}

bool is_command(const CommandsNums num_of_command, const CommandsNums num_of_existing_command, bool *command_exist)
{
    assert(command_exist != NULL);

    if (num_of_command == num_of_existing_command)
    {
        *command_exist = true;
        return true;
    }

    return false;
}