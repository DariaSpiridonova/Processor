#ifndef PROCESSOR
#define PROCESSOR

#include <stdio.h>
#include <assert.h>
#include "/home/dasha/projects/Impenetrable_stack/stack.h"

#define ASSERT(spu, num_of_parameters) \
    assert(spu != NULL);\
    assert(spu->stk != NULL);\
    assert(spu->buffer_with_commands != NULL);\
    assert(spu->buffer_of_registers != NULL);\
    assert(num_of_parameters > 0);
#define PROCESSOR_DUMP(stk, num_of_parameters)\
        ProcessorDump(stk, num_of_parameters, __FILE__, __LINE__)

const size_t SIZE = 16;
const used_type MULTY = 1000;

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
    JMP
};

enum ProcessorErrors
{
    NO_SPU_ERROR,
    READING_BYTE_CODE_ERROR, 
    ERROR_IN_STACK,
    EMPTY_POINTER_ON_STACK,
    EMPTY_POINTER_ON_COMMANDS_BUFFER,
    EMPTY_REGISTERS_BUFFER,
    ERROR_IN_COUNTER,
    INCORRECT_NUMBER_OF_PARAMETERS,

    NON_EXISTENT_COMMAND,
    DIVISION_BY_ZERO,
    THE_ROOT_OF_A_NEGATIVE_NUMBER,
    FORCED_TERMINATION
};

struct processor
{
    struct stack_t *stk;
    int *buffer_with_commands;
    ssize_t instruct_counter;
    used_type buffer_of_registers[SIZE];
};

bool open_files_success(FILE *byte_code);
ProcessorErrors ProcessorInit(struct processor *spu, ssize_t num_of_parameters, FILE *byte_code);
ProcessorErrors ProcessorVerify(struct processor *spu, ssize_t num_of_parameters);
ProcessorErrors ProcessorDestroy(struct processor *spu, ssize_t num_of_parameters);
void ProcessorDump(const struct processor *spu, ssize_t num_of_parameters, const char *file, int line);
ProcessorErrors fill_buffers(struct processor *spu, ssize_t num_of_parameters);

ProcessorErrors ProcessorIntepreter(struct processor *spu, ssize_t num_of_parameters);
ProcessorErrors read_and_do_the_command(struct processor *spu, ssize_t num_of_parameters);
ProcessorErrors do_push(struct processor *spu, ssize_t num_of_parameters);
ProcessorErrors arithmetic_operations(struct processor *spu, int command, ssize_t num_of_parameters);
ProcessorErrors do_sqrt(struct processor *spu, ssize_t num_of_parameters);
ProcessorErrors do_out(struct processor *spu, ssize_t num_of_parameters);
ProcessorErrors do_hlt(struct processor *spu, ssize_t num_of_parameters);
ProcessorErrors do_pushr(struct processor *spu, ssize_t num_of_parameters);
ProcessorErrors do_popr(struct processor *spu, ssize_t num_of_parameters);
ProcessorErrors do_j(struct processor *spu, ssize_t num_of_parameters, CommandsNums command);
ProcessorErrors do_jmp(struct processor *spu, ssize_t num_of_parameters);
ProcessorErrors do_in(struct processor *spu, ssize_t num_of_parameters);

bool print_error(ProcessorErrors err);

#endif