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
    NON_EXISTING_COMMAND
};

struct commands_parameters
{
    char command1[SIZE];
    char command2[SIZE];
    int argument1;
    int argument2;
    size_t num_of_strings;
};

bool open_files_success(FILE *original_commands, FILE *received_commands);

#endif