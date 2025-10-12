#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "calculator.h"
#include "stack.h"

CalculateErrors read_and_do_command(struct stack_t *stk, FILE *commands, struct calculator_parameters parampampa)
{
    if (fscanf(commands, "%s", parampampa.command) != 1)
    {
        return ERROR_IN_READING_THE_COMMAND;
    }

    CalculateErrors execution_result = DATA_OK;
    if      (is_command(parampampa.command, "PUSH")) execution_result = do_push(stk, commands);
    else if (is_command(parampampa.command, "ADD" )) execution_result = arithmetic_operations(stk, "ADD");
    else if (is_command(parampampa.command, "SUB" )) execution_result = arithmetic_operations(stk, "SUB");
    else if (is_command(parampampa.command, "MUL" )) execution_result = arithmetic_operations (stk, "MUL");
    else if (is_command(parampampa.command, "DIV" )) execution_result = arithmetic_operations (stk, "DIV");
    else if (is_command(parampampa.command, "SQRT")) execution_result = do_sqrt(stk);
    else if (is_command(parampampa.command, "OUT" )) execution_result = do_out (stk);
    else if (is_command(parampampa.command, "HLT" )) execution_result = do_hlt (stk);
    else 
        return NON_EXISTENT_COMMAND;
    
    return execution_result;
}

bool is_command(const char *received_command, const char *existing_command)
{
    assert(received_command != NULL && existing_command != NULL);

    return !strcmp(received_command, existing_command);
}

CalculateErrors do_push(struct stack_t *stk, FILE *commands)
{
    ASSERTS(stk);
    
    used_type deliver_value = 0;
    if (fscanf(commands, "%d", &deliver_value) != 1)
    {
        return ABSENCE_NUM;
    }

    if (StackPush(stk, deliver_value*MULTY)) return ERROR_IN_STACK;

    return DATA_OK;
}

CalculateErrors arithmetic_operations(struct stack_t *stk, const char *existing_command)
{
    ASSERTS(stk);
    
    used_type summands[2] = {0, 0};

    for (int i = 0; i < 2; i++)
    {
        StackErr_t err = NO_ERROR;
        summands[i] = StackPop(stk, &err);
        if ((err = StackVerify(stk))) 
        {
            StackDump(stk,__FILE__, __LINE__); 
            return ERROR_IN_STACK;
        }
    }

    used_type result = 0;
    if (!strcmp("ADD", existing_command)) result = summands[0] + summands[1];
    if (!strcmp("SUB", existing_command)) result = summands[0] - summands[1];
    if (!strcmp("MUL", existing_command)) result = summands[0]*summands[1]/MULTY;
    if (!strcmp("DIV", existing_command)) 
    {
        if (summands[1] == 0)
        {
            return DIVISION_BY_ZERO;
        }
        result = summands[0]*MULTY/summands[1];
    }

    printf("result before push = %d\n", result);

    if (StackPush(stk, result)) return ERROR_IN_STACK;;

    return DATA_OK;
}

CalculateErrors do_sqrt(struct stack_t *stk)
{
    ASSERTS(stk);

    StackErr_t err = NO_ERROR;
    used_type deliver_value = StackPop(stk, &err);

    if (deliver_value < 0) 
    {
        return THE_ROOT_OF_A_NEGATIVE_NUMBER;
    } 

    if ((err = StackVerify(stk))) 
    {
        StackDump(stk,__FILE__, __LINE__); 
        return ERROR_IN_STACK;
    }
    
    if (StackPush(stk, (used_type)pow(deliver_value*MULTY, 0.5))) return ERROR_IN_STACK;;

    return DATA_OK;
}

CalculateErrors do_out(struct stack_t *stk)
{
    ASSERTS(stk);

    StackErr_t err = NO_ERROR;
    used_type deliver_value = StackPop(stk, &err);
    if ((err = StackVerify(stk))) 
    {
        StackDump(stk,__FILE__, __LINE__); 
        return ERROR_IN_STACK;
    }

    printf("The value obtained from the stack: %d\n", deliver_value/MULTY);

    return DATA_OK;
}

CalculateErrors do_hlt(struct stack_t *stk)
{
    ASSERTS(stk);

    StackErr_t err = NO_ERROR;
    if ((err = StackVerify(stk)))
    {
        STACK_DUMP(stk);
        return ERROR_IN_STACK;
    }

    return FORCED_TERMINATION;
}

bool print_error(CalculateErrors error)
{
    switch (error)
    {
        case DATA_OK:
            printf("Everything is correct.\n");
            return true;

        case ERROR_IN_READING_THE_COMMAND:
            printf("The command was not read.\n");
            return true;

        case ABSENCE_NUM:
            printf("No number is passed to the command that requires the parameter.\n");
            return true;

        case NON_EXISTENT_COMMAND:
            printf("The entered command is incorrect and does not exist.\n");
            return true;

        case TOO_HIGH_VALUE:
            printf("The number passed to the command is too high.\n");
            return true;

        case DIVISION_BY_ZERO:
            printf("Division by zero is unacceptable.\n");
            return true;

        case THE_ROOT_OF_A_NEGATIVE_NUMBER:
            printf("There is no real number equal to the square root of a negative number.\n");
            return true;

        case ERROR_IN_STACK:
            printf("There is a problem in the stack.\n");
            return true;

        case NO_SUCH_FILE:
            printf("There is no such file for reading commands.\n");
            return true;

        case LACK_OF_COMMANDS:
            printf("There is empty file without any commands.\n");
            return true;

        case NON_EXISTENT_ERROR:
            return false;

        case FORCED_TERMINATION:
            return false;
    
    default:
        return false;
    }
}