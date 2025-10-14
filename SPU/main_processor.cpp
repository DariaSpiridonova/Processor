#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "processor.h"

int main()
{
    FILE *byte_code = fopen("../Assembler/byte_code.txt", "r");

    if (!open_files_success(byte_code))
        return -1;

    ssize_t num_of_parameters = 0;
    if (fscanf(byte_code, "%ld", &num_of_parameters) != 1)
    {
        printf("The file \"byte_code.txt\" contains incorrect data");
        return 1;
    }

    struct stack_t stk;

    struct processor spu = 
    {
        .stk = &stk
    };

    ProcessorErrors err = ProcessorInit(&spu, num_of_parameters, byte_code);
    if (err)
    {
        if (!print_error(err))
        {
            printf("Error when printing errors during initialization");
            return 2;
        };
        return 3;
    }

    err = ProcessorIntepreter(&spu, num_of_parameters);
    if (err)
    {
        printf("%d\n", (int)err);
        if (!print_error(err))
        {
            printf("Error when printing errors when interpreting");
            return 4;
        };
        printf("OK\n");
        return 5;
    }
    printf("OK\n");
    err = ProcessorDestroy(&spu, num_of_parameters);
    if (err)
    {
        if (!print_error(err))
        {
            printf("Error during memory cleanup");
            return 6;
        };
        return 7;
    }

    return 0;
}

bool open_files_success(FILE *byte_code)
{
    if (byte_code == NULL)
    {
        printf("An error occurred when opening the file \"byte_code.txt\"");
        return false;
    }

    return true;
}