#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "assembly.h"

bool identify_the_command(struct commands_for_change *data_buffer)
{
    bool command_exist = false;

    if      (is_command(data_buffer->command, "PUSH",  &command_exist)) data_buffer->num_of_command = PUSH;
    else if (is_command(data_buffer->command, "ADD",   &command_exist)) data_buffer->num_of_command = ADD;
    else if (is_command(data_buffer->command, "SUB",   &command_exist)) data_buffer->num_of_command = SUB;
    else if (is_command(data_buffer->command, "MUL",   &command_exist)) data_buffer->num_of_command = MUL;
    else if (is_command(data_buffer->command, "DIV",   &command_exist)) data_buffer->num_of_command = DIV;
    else if (is_command(data_buffer->command, "SQRT",  &command_exist)) data_buffer->num_of_command = SQRT;
    else if (is_command(data_buffer->command, "OUT",   &command_exist)) data_buffer->num_of_command = OUT;
    else if (is_command(data_buffer->command, "HLT",   &command_exist)) data_buffer->num_of_command = HLT; 
    else if (is_command(data_buffer->command, "PUSHR", &command_exist)) data_buffer->num_of_command = PUSHR; 
    else if (is_command(data_buffer->command, "POPR",  &command_exist)) data_buffer->num_of_command = POPR; 
    else if (is_command(data_buffer->command, "JB",    &command_exist)) data_buffer->num_of_command = JB; 
    else if (is_command(data_buffer->command, "JA",    &command_exist)) data_buffer->num_of_command = JA; 
    else if (is_command(data_buffer->command, "JBE",    &command_exist)) data_buffer->num_of_command = JBE; 
    else if (is_command(data_buffer->command, "JAE",    &command_exist)) data_buffer->num_of_command = JAE; 
    else if (is_command(data_buffer->command, "JE",    &command_exist)) data_buffer->num_of_command = JE; 
    else if (is_command(data_buffer->command, "JNE",    &command_exist)) data_buffer->num_of_command = JNE; 
    else if (is_command(data_buffer->command, "JMP",   &command_exist)) data_buffer->num_of_command = JMP; 
    else if (is_command(data_buffer->command, "IN",    &command_exist)) data_buffer->num_of_command = IN; 
    
    return command_exist;
}

int get_argument(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer)
{
    switch (data_buffer->num_of_command)
        {
        case PUSH:
            if (fscanf(commands_for_reading, "%d", &data_buffer->argument) != 1)
            {
                printf("\033[31min commands.asm: line %zu: The %s command argument is missing.\033[0m\n", data_buffer->num_of_strings, data_buffer->command);
                return 3;
            }
            fprintf(commands_for_record, "%d\n", data_buffer->argument);
            data_buffer->num_of_parameters++;
            break;
        
        case PUSHR:
        case POPR:
            if (fscanf(commands_for_reading, "%s", data_buffer->reg_argument) != 1)
            {
                printf("\033[31min commands.asm: line %zu: The %s command argument is missing.\033[0m\n", data_buffer->num_of_strings, data_buffer->command);
                return 4;
            }
            if (!def_reg_arg(data_buffer))
            {
                printf("\033[31min commands.asm: line %zu: The %s command argument is incorrect.\033[0m\n", data_buffer->num_of_strings, data_buffer->command);
                return 5;
            };

            fprintf(commands_for_record, "%d\n", (int)data_buffer->reg_command);
            data_buffer->num_of_parameters++;
            break;

        case JB:
        case JA:
        case JBE:
        case JAE:
        case JE:
        case JNE:
        case JMP:        
            if (fscanf(commands_for_reading, "%d", &data_buffer->argument) != 1)
            {
                printf("\033[31min commands.asm: line %zu: The %s command argument is missing.\033[0m\n", data_buffer->num_of_strings, data_buffer->command);
                return 8;
            }
            fprintf(commands_for_record, "%d\n", data_buffer->argument);
            data_buffer->num_of_parameters++;
            break;
        
        case ADD:
        case DIV:
        case MUL:
        case SUB:
        case SQRT:
        case OUT:
        case HLT:
        case NON_EXISTING_COMMAND:
        case IN:
            fprintf(commands_for_record, "\n");
            break;

        default:
            break;
        }

    data_buffer->num_of_strings++;
    return 0;
}

bool def_reg_arg(struct commands_for_change *data_buffer)
{
    bool command_exist = false;

    if      (is_command(data_buffer->reg_argument, "AX", &command_exist)) data_buffer->reg_command = AX;
    else if (is_command(data_buffer->reg_argument, "BX", &command_exist)) data_buffer->reg_command = BX;
    else if (is_command(data_buffer->reg_argument, "CX", &command_exist)) data_buffer->reg_command = CX;
    else if (is_command(data_buffer->reg_argument, "DX", &command_exist)) data_buffer->reg_command = DX;

    return command_exist;
}

bool is_command(const char *received_command, const char *existing_command, bool *command_exist)
{
    assert(received_command != NULL && existing_command != NULL);

    if (!strcmp(received_command, existing_command))
    {
        *command_exist = true;
        return true;
    }

    return false;
}