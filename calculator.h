#include <stdio.h>
#include "stack.h"

const used_type MULTY = 1000;

enum CalculateErrors 
{
    DATA_OK,
    ERROR_IN_READING_THE_COMMAND,
    ABSENCE_NUM,
    NON_EXISTENT_COMMAND,
    TOO_HIGH_VALUE,
    DIVISION_BY_ZERO,
    THE_ROOT_OF_A_NEGATIVE_NUMBER,
    ERROR_IN_STACK,
    NO_SUCH_FILE,
    LACK_OF_COMMANDS,
    NON_EXISTENT_ERROR,
    FORCED_TERMINATION,
};

struct calculator_parameters
{
    char command[10];
    int argument;
    used_type value;
};

CalculateErrors read_and_do_command(struct stack_t *stk, FILE *commands, struct calculator_parameters parampampa);
bool is_command(const char *received_command, const char *existing_command);
CalculateErrors do_push(struct stack_t *stk, FILE *commands);
CalculateErrors arithmetic_operations(struct stack_t *stk, const char *existing_command);
CalculateErrors do_sqrt(struct stack_t *stk);
CalculateErrors do_out(struct stack_t *stk);
CalculateErrors do_hlt(struct stack_t *stk);
bool print_error(CalculateErrors error);