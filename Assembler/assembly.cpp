#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "assembly.h"

bool identify_the_command(struct commands_for_change *data_buffer)
{
    assert(data_buffer != NULL);

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
    else if (is_command(data_buffer->command, "JBE",   &command_exist)) data_buffer->num_of_command = JBE; 
    else if (is_command(data_buffer->command, "JAE",   &command_exist)) data_buffer->num_of_command = JAE; 
    else if (is_command(data_buffer->command, "JE",    &command_exist)) data_buffer->num_of_command = JE; 
    else if (is_command(data_buffer->command, "JNE",   &command_exist)) data_buffer->num_of_command = JNE; 
    else if (is_command(data_buffer->command, "JMP",   &command_exist)) data_buffer->num_of_command = JMP; 
    else if (is_command(data_buffer->command, "IN",    &command_exist)) data_buffer->num_of_command = IN; 
    else if (is_command(data_buffer->command, "CALL",  &command_exist)) data_buffer->num_of_command = CALL; 
    else if (is_command(data_buffer->command, "RET",   &command_exist)) data_buffer->num_of_command = RET; 
    else if (is_command(data_buffer->command, "PUSHM", &command_exist)) data_buffer->num_of_command = PUSHM; 
    else if (is_command(data_buffer->command, "POPM",  &command_exist)) data_buffer->num_of_command = POPM; 
    else if (data_buffer->command[0] == ':') 
    {
        data_buffer->num_of_command = COLON; 
        command_exist = true;
    }
    else if (data_buffer->command[0] == ';')
    {
        data_buffer->num_of_command = COMMENT; 
        command_exist = true;
    }
    
    return command_exist;
}

int get_argument(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer)
{
    ASSERTS();

    int ch = 0;
    switch (data_buffer->num_of_command)
        {
        case PUSH:
            if ((ch = check_and_get_arg(commands_for_reading, commands_for_record, data_buffer)))
                return ch;
            break;
        
        case PUSHR:
        case POPR:
            if ((ch = check_and_get_reg(commands_for_reading, commands_for_record, data_buffer)))
                return ch;
            break;

        case JB:
        case JA:
        case JBE:
        case JAE:
        case JE:
        case JNE:
        case JMP:     
        case CALL: 
            if ((ch = check_and_get_label_argument(commands_for_reading, commands_for_record, data_buffer)))
                return ch;
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
        case RET:
            fprintf(commands_for_record, "\n");
            break;

        case PUSHM:
        case POPM:
            if ((ch = check_and_get_index(commands_for_reading, commands_for_record, data_buffer)))
                return ch;
            break;

        case COLON:
            if ((ch = check_and_get_label(data_buffer)))
                return ch;
            break;

        case COMMENT:
            if ((ch = skip(commands_for_reading, data_buffer)))
                return ch;
            break;

        default:
            break;
        }

    data_buffer->num_of_strings++;
    return 0;
}

bool open_files_success(FILE *fp, size_t file_num)
{
    if (fp == NULL && file_num == 0)
    {
        printf("An error occurred when opening the file \"original_commands.asm\"");
        return false;
    }

    if (fp == NULL && file_num == 1)
    {
        printf("An error occurred when opening the file \"byte_code.txt\"");
        return false;
    }

    return true;
}

bool close_files_success(FILE *commands_for_reading, FILE *commands_for_record)
{
    if (fclose(commands_for_reading) == EOF)
    {
        printf("An error occurred when closing the file \"original_commands.asm\"");
        return false;
    }
    if (fclose(commands_for_record) == EOF)
    {
        printf("An error occurred when closing the file \"byte_code.txt\"");
        return false;
    }

    return true;
}
bool def_reg_arg(struct commands_for_change *data_buffer)
{
    bool command_exist = false;

    if      (is_command(data_buffer->reg_argument, "AX", &command_exist)) data_buffer->reg_command = AX;
    else if (is_command(data_buffer->reg_argument, "BX", &command_exist)) data_buffer->reg_command = BX;
    else if (is_command(data_buffer->reg_argument, "CX", &command_exist)) data_buffer->reg_command = CX;
    else if (is_command(data_buffer->reg_argument, "DX", &command_exist)) data_buffer->reg_command = DX;
    else if (is_command(data_buffer->reg_argument, "SP", &command_exist)) data_buffer->reg_command = SP;
    else if (is_command(data_buffer->reg_argument, "BP", &command_exist)) data_buffer->reg_command = BP;
    else if (is_command(data_buffer->reg_argument, "SI", &command_exist)) data_buffer->reg_command = SI;
    else if (is_command(data_buffer->reg_argument, "DI", &command_exist)) data_buffer->reg_command = DI;

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

int check_and_get_label_argument(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer)
{
    ASSERTS();

    int ch = 0;
    // TODO:
    // if (первый символ :) -> buffer++, fscanf(...)
    // else fscanf(...)
    if (fscanf(commands_for_reading, "%d", &data_buffer->argument) != 1)
    {
        if ((ch = fgetc(commands_for_reading)) != ':')
        {
            printf(RED_COLOR_BEGIN "in commands.asm: line %zu: The %s command argument is missing." RED_COLOR_END, data_buffer->num_of_strings, data_buffer->command);
            return 6;
        }
        if (fscanf(commands_for_reading, "%d", &data_buffer->argument) != 1)
        {
            printf(RED_COLOR_BEGIN "in commands.asm: line %zu: The %s command argument is missing." RED_COLOR_END, data_buffer->num_of_strings, data_buffer->command);
            return 7;
        }
        if (data_buffer->argument < 0) 
        {
            printf(RED_COLOR_BEGIN "in commands.asm: line %zu: The %s command argument is negative." RED_COLOR_END, data_buffer->num_of_strings, data_buffer->command);
            return 8;
        }
        if (data_buffer->labels[data_buffer->argument] == -1 && data_buffer->number_of_compilation == 2)
        {
            printf(RED_COLOR_BEGIN "in commands.asm: line %zu: There is no label with the number %d." RED_COLOR_END, data_buffer->num_of_strings, data_buffer->argument);
            return 9;
        }
        if (data_buffer->labels[data_buffer->argument] != -1)
        {
            fprintf(commands_for_record, "%zd\n", data_buffer->labels[data_buffer->argument]);
            printf("номер команды: %zd\n", data_buffer->labels[data_buffer->argument]);
            printf("номер метки: %d\n", data_buffer->argument);
        }
    }
    else fprintf(commands_for_record, "%zd\n", data_buffer->labels[data_buffer->argument]);
    printf("номер команды: %zd\n", data_buffer->labels[data_buffer->argument]);
    printf("номер метки: %d\n", data_buffer->argument);
    data_buffer->num_of_parameters++;

    return 0;
}

int check_and_get_reg(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer)
{
    ASSERTS();

    if (fscanf(commands_for_reading, "%s", data_buffer->reg_argument) != 1)
    {
        printf(RED_COLOR_BEGIN "in commands.asm: line %zu: The %s command argument is missing." RED_COLOR_END, data_buffer->num_of_strings, data_buffer->command);
        return 4;
    }
    if (!def_reg_arg(data_buffer))
    {
        printf(RED_COLOR_BEGIN "in commands.asm: line %zu: The %s command argument is incorrect." RED_COLOR_END, data_buffer->num_of_strings, data_buffer->command);
        return 5;
    };

    fprintf(commands_for_record, "%d\n", (int)data_buffer->reg_command);
    data_buffer->num_of_parameters++;

    return 0;
}

int check_and_get_arg(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer)
{
    ASSERTS();

    if (fscanf(commands_for_reading, "%d", &data_buffer->argument) != 1)
    {
        printf(RED_COLOR_BEGIN "in commands.asm: line %zu: The %s command argument is missing." RED_COLOR_END, data_buffer->num_of_strings, data_buffer->command);
        return 3;
    }
    fprintf(commands_for_record, "%d\n", data_buffer->argument);
    data_buffer->num_of_parameters++;

    return 0;
}

int check_and_get_label(struct commands_for_change *data_buffer)
{
    if (sscanf(data_buffer->command + 1, "%d", &data_buffer->argument) != 1)
    {
        printf(RED_COLOR_BEGIN "in commands.asm: line %zu: The number from the label is missing." RED_COLOR_END, data_buffer->num_of_strings);
        return 10;
    }
    data_buffer->labels[data_buffer->argument] = data_buffer->num_of_parameters;
    printf("номер команды: %zd\n", data_buffer->labels[data_buffer->argument]);
    
    return 0;
}

int skip(FILE *commands_for_reading, struct commands_for_change *data_buffer)
{
    int ch = 0;
    do
    {
        ch = fgetc(commands_for_reading);
    } while (ch != '\n' && ch != EOF);
    
    if (ch == EOF && !feof(commands_for_reading))
    {
        printf(RED_COLOR_BEGIN "in commands.asm: line %zu: A character reading error has occurred." RED_COLOR_END, data_buffer->num_of_strings);
        return 11;
    }

    return 0;
}

int check_and_get_index(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer)
{
    ASSERTS();

    if (fscanf(commands_for_reading, "[%2s]", data_buffer->reg_argument) != 1)
    {
        printf(RED_COLOR_BEGIN "in commands.asm: line %zu: It is not possible to add an item from RAM to the stack at an incorrectly set index." RED_COLOR_END, data_buffer->num_of_strings);
        return 12;
    }

    if (!def_reg_arg(data_buffer))
    {
        printf(RED_COLOR_BEGIN "in commands.asm: line %zu: The %s command argument in brackets is incorrect." RED_COLOR_END, data_buffer->num_of_strings, data_buffer->command);
        return 13;
    };

    fprintf(commands_for_record, "%d\n", (int)data_buffer->reg_command);
    data_buffer->num_of_parameters++;

    return 0;
}

void labels_init(struct commands_for_change *data_buffer)
{
    assert(data_buffer != NULL);
    assert(data_buffer->labels != NULL);
    assert(SIZE_LABELS_BUFFER >= 0);

    for (size_t i = 0; i < SIZE_LABELS_BUFFER; i++)
    {
        data_buffer->labels[i] = -1;
    }
}

int opening_files_and_initializing_data(FILE **commands_for_reading, FILE **commands_for_record, struct commands_for_change *data_buffer, size_t num_of_compilation)
{
    *commands_for_reading = fopen("original_commands.asm", "r");
    *commands_for_record = fopen("byte_code.txt", "w");

    if (!open_files_success(*commands_for_reading, 0) || !open_files_success(*commands_for_reading, 1))
        return -1;

    for (size_t i = 0; i < SIZE_INPUT; i++)
    {
        data_buffer->command[i] = 0;
        data_buffer->reg_argument[i] = 0;
    }

    data_buffer->num_of_command = NON_EXISTING_COMMAND;
    data_buffer->reg_command = NON_EXISTING_ARGUMENT;
    data_buffer->argument = 0;
    data_buffer->num_of_parameters = 0;
    data_buffer->num_of_strings = 1;
    data_buffer->number_of_compilation = num_of_compilation;

    ASSERTS();

    return 0;
}

int compile_file_content(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer)
{
    int refund = 0;

    while (!feof(commands_for_reading))
    {
        if ((refund = read_command(commands_for_reading, data_buffer)))
        {
            return refund;
        }

        if ((refund = writing_to_a_file(commands_for_reading, commands_for_record, data_buffer)))
        {
            printf(RED_COLOR_BEGIN "in commands.asm: line %zu: Couldn't write command to a file.\n" RED_COLOR_END, data_buffer->num_of_strings);
            return refund;
        }
    }

    return 0;
}

int read_command(FILE *commands_for_reading, struct commands_for_change *data_buffer)
{
    printf("%s\n", data_buffer->command);
    if (fscanf(commands_for_reading, "%9s", data_buffer->command) != 1)
    {
        int ch = fgetc(commands_for_reading);
        printf("%s\n", data_buffer->command);
        printf("%c %d\n", ch, ch);
        printf(RED_COLOR_BEGIN "in commands.asm: line %zu: The command is not set.\n" RED_COLOR_END, data_buffer->num_of_strings);
        return 1;
    }

    if (!identify_the_command(data_buffer))
    {
        printf(RED_COLOR_BEGIN "in commands.asm: line %zu: The command %s does not exist.\n" RED_COLOR_END, data_buffer->num_of_strings, data_buffer->command);
        return 2;
    }

    return 0;
}

int writing_to_a_file(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer)
{
    int res = 0;

    if (data_buffer->num_of_command != COLON && data_buffer->num_of_command != COMMENT)
    {
        fprintf(commands_for_record, "%d ", data_buffer->num_of_command);
        data_buffer->num_of_parameters++;
    }

    if ((res = get_argument(commands_for_reading, commands_for_record, data_buffer)))
        return res; 
    
    return 0;
}

