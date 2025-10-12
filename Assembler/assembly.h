#ifndef ASSEMBLY
#define ASSEMBLY

#include <stdio.h>

#define SIZE 10
enum CommandsNums
{
    PUSH,
    ADD,
    DIV,
    MUL,
    SUB,
    SQRT,
    OUT,
    HLT,
    IN,
    PUSHR = 33,
    POPR = 42,
    JB = 50,
    JA,
    JBE,
    JAE,
    JE,
    JNE,
    JMP,
    NON_EXISTING_COMMAND = -1
};

enum RegistersCommands
{
    AX,
    BX,
    CX,
    DX,
    NON_EXISTING_ARGUMENT
};

struct commands_for_change
{
    char command[SIZE];
    CommandsNums num_of_command;
    RegistersCommands reg_command;
    char reg_argument[SIZE];
    int argument;
    size_t num_of_parameters;
    size_t num_of_strings;
};

bool identify_the_command(struct commands_for_change *data_buffer);
bool def_reg_arg(struct commands_for_change *data_buffer);
bool is_command(const char *received_command, const char *existing_command, bool *command_exist);
int get_argument(FILE *commands_for_reading, FILE *commands_for_record, struct commands_for_change *data_buffer);
#endif