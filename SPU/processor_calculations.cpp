#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "processor.h"

ProcessorErrors ProcessorIntepreter(struct processor *spu, ssize_t num_of_parameters)
{
    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    while (spu->instruct_counter < num_of_parameters)
    {
        if ((err = read_and_do_the_command(spu, num_of_parameters)))
        {
            if (err == FORCED_TERMINATION)
            {
                err = NO_SPU_ERROR; 
                break;
            }
            return err;
        }
    }

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    return err;
}

ProcessorErrors read_and_do_the_command(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    CommandsNums command = (CommandsNums)spu->buffer_with_commands[spu->instruct_counter];
    switch(command)
    {
        case PUSH:
            err = do_push(spu, num_of_parameters);
            break;

        case ADD:
        case DIV:
        case MUL:
        case SUB:
            err = arithmetic_operations(spu, command, num_of_parameters);
            break;

        case SQRT:
            err = do_sqrt(spu, num_of_parameters);
            break;

        case OUT:
            err = do_out(spu, num_of_parameters);
            break;

        case HLT:
            err = do_hlt(spu, num_of_parameters);
            break;

        case PUSHR:
            err = do_pushr(spu, num_of_parameters);
            break;

        case POPR:
            err = do_popr(spu, num_of_parameters);
            break;

        case IN:
            err = do_in(spu, num_of_parameters);
            break;
        case JB:
        case JA:
        case JBE:
        case JAE:
        case JE:
        case JNE:
            err = do_j(spu, num_of_parameters, command);
            if (err) break;
            goto transition; 
        
        case JMP:
            err = do_jmp(spu, num_of_parameters);
            if (err) break;
            goto transition; 

        default:
            return NON_EXISTENT_COMMAND;
    }
    
    if (err) return err;

    spu->instruct_counter++;

    transition:
    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    return err;
}

ProcessorErrors do_push(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }
    
    used_type deliver_value = spu->buffer_with_commands[++spu->instruct_counter];

    if (StackPush(spu->stk, deliver_value*MULTY)) return ERROR_IN_STACK;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    return NO_SPU_ERROR;
}

ProcessorErrors arithmetic_operations(struct processor *spu, int command, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    used_type summands[2] = {0, 0};

    StackErr_t err1 = NO_ERROR;
    for (int i = 0; i < 2; i++)
    {
        err1 = NO_ERROR;
        summands[i] = StackPop(spu->stk, &err1);
        if ((err1 = StackVerify(spu->stk))) 
        {
            StackDump(spu->stk,__FILE__, __LINE__); 
            return ERROR_IN_STACK;
        }
    }

    used_type result = 0;
    if (command == ADD) result = summands[0] + summands[1];
    if (command == SUB) result = summands[1] - summands[0];
    if (command == MUL) result = summands[0]*summands[1]/MULTY;
    if (command == DIV) 
    {
        if (summands[0] == 0)
        {
            return DIVISION_BY_ZERO;
        }
        result = summands[1]*MULTY/summands[0];
    }

    if (StackPush(spu->stk, result)) return ERROR_IN_STACK;
    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    return NO_SPU_ERROR;
}

ProcessorErrors do_sqrt(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    StackErr_t err1 = NO_ERROR;
    used_type deliver_value = StackPop(spu->stk, &err1);

    if (deliver_value < 0) 
    {
        return THE_ROOT_OF_A_NEGATIVE_NUMBER;
    } 

    if ((err1 = StackVerify(spu->stk))) 
    {
        StackDump(spu->stk,__FILE__, __LINE__); 
        return ERROR_IN_STACK;
    }
    
    if (StackPush(spu->stk, (used_type)pow(deliver_value*MULTY, 0.5))) return ERROR_IN_STACK;
    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    return NO_SPU_ERROR;
}

ProcessorErrors do_out(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    StackErr_t err1 = NO_ERROR;
    used_type deliver_value = StackPop(spu->stk, &err1);
    if ((err1 = StackVerify(spu->stk))) 
    {
        StackDump(spu->stk,__FILE__, __LINE__); 
        return ERROR_IN_STACK;
    }

    printf("The value obtained from the stack: %d\n", deliver_value/MULTY);

    return NO_SPU_ERROR;
}

ProcessorErrors do_hlt(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    StackErr_t err1 = NO_ERROR;
    if ((err1 = StackVerify(spu->stk)))
    {
        STACK_DUMP(spu->stk);
        return ERROR_IN_STACK;
    }

    return FORCED_TERMINATION;
}

ProcessorErrors do_pushr(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    int index = spu->buffer_with_commands[++spu->instruct_counter];

    if (StackPush(spu->stk, spu->buffer_of_registers[index])) return ERROR_IN_STACK;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    return NO_SPU_ERROR;

}

ProcessorErrors do_popr(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    StackErr_t err1 = NO_ERROR;
    used_type deliver_value = StackPop(spu->stk, &err1);
    if ((err1 = StackVerify(spu->stk))) 
    {
        StackDump(spu->stk,__FILE__, __LINE__); 
        return ERROR_IN_STACK;
    }

    int index = spu->buffer_with_commands[++spu->instruct_counter];
    spu->buffer_of_registers[index] = deliver_value;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    return err;
}

ProcessorErrors do_j(struct processor *spu, ssize_t num_of_parameters, CommandsNums command)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    used_type values[] = {0, 0};

    StackErr_t err1 = NO_ERROR;
    for (size_t i = 0; i < 2; i++)
    {
        values[i] = StackPop(spu->stk, &err1);
        if ((err1 = StackVerify(spu->stk))) 
        {
            StackDump(spu->stk,__FILE__, __LINE__); 
            return ERROR_IN_STACK;
        }
    } 

    bool res_of_compare = false;

    switch(command)
    {
        case JB:
            res_of_compare = values[0] < values[1];
            break;

        case JA:
            res_of_compare = values[0] > values[1];
            break;

        case JBE:
            res_of_compare = values[0] <= values[1];
            break;

        case JAE:
            res_of_compare = values[0] >= values[1];
            break;

        case JE:
            res_of_compare = (values[0] == values[1]);
            break;

        case JNE:
            res_of_compare = values[0] != values[1];
            break;

        case PUSH:
        case ADD:
        case DIV:
        case MUL:
        case SUB:
        case SQRT:
        case OUT:
        case HLT:
        case PUSHR:
        case POPR:
        case JMP:
        case IN:
            return NON_EXISTENT_COMMAND;
        default:
            return NON_EXISTENT_COMMAND;
    }

    if (res_of_compare)
    {
        int adress = spu->buffer_with_commands[++spu->instruct_counter];
        spu->instruct_counter = (ssize_t)adress;
    }
    else spu->instruct_counter += 2;
    
    return err;
}

ProcessorErrors do_jmp(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    int adress = spu->buffer_with_commands[++spu->instruct_counter];
    spu->instruct_counter = (ssize_t)adress;

    return err;
}

ProcessorErrors do_in(struct processor *spu, ssize_t num_of_parameters)
{
    ASSERT(spu, num_of_parameters);

    ProcessorErrors err = NO_SPU_ERROR;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    used_type console_value = 0;
    while (scanf("%d", &console_value) != 1)
    {
        printf("Please, enter an integer\n");
        int ch = 0;
        while ((ch = getchar()) != '\n') continue;
    }

    if (StackPush(spu->stk, console_value*MULTY)) return ERROR_IN_STACK;

    if ((err = ProcessorVerify(spu, num_of_parameters)))
    {
        PROCESSOR_DUMP(spu, num_of_parameters);
        return err;
    }

    return err;
}