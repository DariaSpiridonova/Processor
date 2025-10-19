#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "processor.h"

ProcessorErrors ProcessorInit(struct processor *spu, ssize_t num_of_parameters, FILE *byte_code)
{
    assert(spu != NULL);
    assert(byte_code != NULL);
    assert(num_of_parameters > 0);

    spu->buffer_with_commands = (int *)calloc((size_t)num_of_parameters + 1, sizeof(int));
    if (spu->buffer_with_commands == NULL) return ERROR_IN_STACK;
    spu->RAM = (used_type *)calloc((size_t)SIZE1 + 1, sizeof(int));
    if (spu->RAM == NULL) return ERROR_IN_RAM;

    if (StackInit(spu->stk, num_of_parameters) || StackInit(spu->refund_stk, SIZE1))
        return ERROR_IN_STACK;

    spu->instruct_counter = 0;
    
    if (spu->buffer_with_commands == NULL)
    {
        return EMPTY_POINTER_ON_COMMANDS_BUFFER;
    }
    if (spu->stk == NULL)
    {
        return EMPTY_POINTER_ON_STACK;
    }

    ProcessorErrors err = NO_SPU_ERROR;
    if ((err = fill_buffers(spu, num_of_parameters)))
    {
        return err;
    };

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    for (ssize_t i = 0; i < num_of_parameters; i++)
    {
        if (fscanf(byte_code, "%d", &spu->buffer_with_commands[i]) != 1)
        {
            return READING_BYTE_CODE_ERROR;
        }
    }

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    ASSERT(spu, num_of_parameters);

    return err;
}

ProcessorErrors ProcessorVerify(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    StackErr_t err = NO_ERROR;
    if ((err = StackVerify(spu->stk)))
    {
        STACK_DUMP(spu->stk);
        return ERROR_IN_STACK;
    }

    if (spu->stk == NULL) return EMPTY_POINTER_ON_STACK;
    else if (spu->buffer_with_commands == NULL) return EMPTY_POINTER_ON_COMMANDS_BUFFER;
    else if (spu->RAM == NULL) return EMPTY_POINTER_ON_RAM;
    else if (sizeof(spu->buffer_of_registers) == 0) return EMPTY_REGISTERS_BUFFER;
    else if (spu->instruct_counter < 0) return ERROR_IN_COUNTER;
    else if (num_of_parameters <= 0) return INCORRECT_NUMBER_OF_PARAMETERS;

    ASSERT(spu, num_of_parameters);

    return NO_SPU_ERROR;
}

ProcessorErrors ProcessorDestroy(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;
    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }
    printf("FREE\n");
    StackDestroy(spu->stk);
    StackDestroy(spu->refund_stk);
    free(spu->buffer_with_commands);
    free(spu->RAM);
    spu->buffer_with_commands = NULL;
    spu = NULL;

    return err;
}

void ProcessorDump(const struct processor *spu, ssize_t num_of_parameters, const char *file, int line)
{
    ASSERT(spu, num_of_parameters);

    printf("ProcessorDump called from %s :%d\n", file, line);
    printf("SPU[%p]\n", spu);
    printf("BUFFER_WITH_COMMANDS[%p]\n", spu->buffer_with_commands);
    printf("BUFFER_OF_REGISTERS[%p]\n", spu->buffer_of_registers);
    printf("REFUND_STACK[%p]\n", spu->refund_stk);
    printf("RAM[%p]\n", spu->RAM);
    printf("{\n");
    printf("    instruct_counter = %ld", spu->instruct_counter);
    printf("    num_of_parameters = %ld", num_of_parameters);
    printf("Size of buffer with registers = %zu", sizeof(spu->buffer_of_registers));
    printf("    Stack contents:\n");
    for (size_t i = 0; i < SIZE1; i++)
    {
        printf("        [%zu] = %d\n", i, spu->refund_stk->data[i]);
    }
    printf("    Commands written to the buffer:\n");
    for (ssize_t i = 0; i < num_of_parameters; i++)
    {
        printf("        [%zd] = %d\n", i, spu->buffer_with_commands[i]);
    }
    printf("    Buffer with registers:\n");
    for (size_t i = 0; i < SIZE; i++)
    {
        printf("        [%zu] = %d\n", i, spu->buffer_of_registers[i]);
    }
    printf("    RAM:\n");
    for (size_t i = 0; i < SIZE1; i++)
    {
        printf("        [%zu] = %d\n", i, spu->RAM[i]);
    }
    printf("    }\n");
    printf("}\n");
}

bool print_error(ProcessorErrors err)
{
    switch(err)
    {
        case NO_SPU_ERROR:
            return true;

        case READING_BYTE_CODE_ERROR:
            printf("An error occurred while reading the code");
            return true;

        case ERROR_IN_STACK:
            printf("An error occurred while working with the stack");
            return true;
        
        case ERROR_IN_RAM:
            printf("An error occurred while working with the RAM");
            return true;

        case EMPTY_POINTER_ON_STACK:
            printf("The pointer to the stack is empty");
            return true;

        case EMPTY_POINTER_ON_COMMANDS_BUFFER:
            printf("The pointer to the buffer with commands is empty");
            return true;

        case EMPTY_REGISTERS_BUFFER:
            printf("The pointer to the buffer with registers is empty");
            return true;

        case EMPTY_POINTER_ON_RAM:
            printf("The pointer to the RAM is empty");
            return true;

        case ERROR_IN_COUNTER:
            printf("Instruction counter takes a negative value");
            return true;

        case DIVISION_BY_ZERO:
            printf("Division by zero is unacceptable.\n");
            return true;

        case THE_ROOT_OF_A_NEGATIVE_NUMBER:
            printf("There is no real number equal to the square root of a negative number.\n");
            return true;
        
        case INCORRECT_NUMBER_OF_PARAMETERS:
            printf("Number of parameters is incorrect");
            return true;

        case NON_EXISTENT_COMMAND:
            printf("A non-existent command was found.");
            return true;

        case FORCED_TERMINATION:
            return true;

        default:
            return false;
    }

    return false;
}

ProcessorErrors fill_buffers(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);
    ProcessorErrors err = NO_SPU_ERROR;
    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    for (ssize_t i = 0; i < num_of_parameters + 1; i++)
    {
        spu->buffer_with_commands[i] = POIZON;
    }
    for (size_t i = 0; i < SIZE; i++)
    {
        spu->buffer_of_registers[i] = POIZON;
    }

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    return err;
}