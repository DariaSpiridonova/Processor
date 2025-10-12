#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "assembly.h"

int main()
{
    FILE *commands_for_reading = fopen("original_commands.asm", "r");
    FILE *commands_for_record = fopen("byte_code.txt", "w");

    struct commands_for_change data_buffer = 
    {
        .command = {0},
        .num_of_command = NON_EXISTING_COMMAND,
        .reg_command = NON_EXISTING_ARGUMENT,
        .reg_argument = {0},
        .argument = 0,
        .num_of_parameters = 0,
        .num_of_strings = 1
    };

    fprintf(commands_for_record, "                                    \n");
    int res = 0;
    while (!feof(commands_for_reading))
    {
        if (fscanf(commands_for_reading, "%9s", data_buffer.command) != 1)
        {
            printf("\033[31min commands.asm: line %zu: The command is not set.\033[0m\n", data_buffer.num_of_strings);
            return 1;
        }

        if (!identify_the_command(&data_buffer))
        {
            printf("\033[31min commands.asm: line %zu: The command %s does not exist.\033[0m\n", data_buffer.num_of_strings, data_buffer.command);
            return 2;
        }

        fprintf(commands_for_record, "%d ", data_buffer.num_of_command);
        data_buffer.num_of_parameters++;

        if ((res = get_argument(commands_for_reading, commands_for_record, &data_buffer)))
            return res;   
    }

    fseek(commands_for_record, 0, SEEK_SET);
    fprintf(commands_for_record, "%zu", data_buffer.num_of_parameters++);
    return 0;
}