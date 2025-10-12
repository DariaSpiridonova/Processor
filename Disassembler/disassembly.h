#ifndef DISASSEMBLY
#define DISASSEMBLY

#include <stdio.h>

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
    NON_EXISTING_COMMAND
};

struct commands_for_change
{
    const char *command;
    CommandsNums num_of_command;
    int argument;
    size_t num_of_parameters;
    size_t num_of_strings;
};

bool identify_the_command(struct commands_for_change *data_remember);
bool is_command(const CommandsNums num_of_command, const CommandsNums num_of_existing_command, bool *command_exist);

#endif