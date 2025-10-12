#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "calculator.h"

int main()
{
    struct stack_t stk1 = {};
    StackInit(&stk1, 16);

    FILE *commands = fopen("byte_code.txt", "r");
    if (commands == NULL) 
    {
        printf("There is no such file for reading commands.\n");
        StackDestroy(&stk1);
        return NO_SUCH_FILE;
    }

    else if (feof(commands))
    {
        printf("There is empty file without any commands.\n");
        StackDestroy(&stk1);
        return LACK_OF_COMMANDS;
    }

    struct calculator_parameters calculator = 
    {
        .command = {0},
        .argument = 0,
        .value = POIZON
    };

    while (!feof(commands))
    {
        CalculateErrors error = DATA_OK;

        if ((error = read_and_do_command(&stk1, commands, calculator)) != DATA_OK && error != FORCED_TERMINATION)
        {
            bool success_print_err = print_error(error);
            if (success_print_err == false) 
            {
                printf("A non-existent error was found.\n");
                StackDestroy(&stk1);
                return NON_EXISTENT_ERROR;
            }
        }
        else if (error == FORCED_TERMINATION)
        {
            StackDestroy(&stk1);
            return 0;
        }
    }

    StackDestroy(&stk1);
    /*
    printf("Enter the desired number of items in the stack: ");
    
    while (scanf("%zd", &stk1.capacity) != 1 || stk1.capacity < 0)
    {
        printf("You must enter the natural number: ");
        int ch = 0;
        while((ch = getchar()) != '\n') 
            continue;
    }

    StackInit(&stk1, stk1.capacity);

    printf("Enter the numbers you want to add to the stack(a non-numeric character for completion): ");
    used_type value = 0;

    while(scanf("%d", &value) == 1)
    {
        StackPush(&stk1, value);
    }
    
    int ch = 0;
    while((ch = getchar()) != '\n') 
        continue;

    StackErr_t err = NO_ERROR;
    for (ssize_t i = stk1.size; i > 0; i--)
    {
        used_type deliver_value = StackPop(&stk1, &err);
        if ((err = StackVerify(stk))) 
        {
            StackDump(&stk1,__FILE__, __LINE__); // можешь впихнуть в stackverify
            printf("ошибка: %d", err);
        }
        printf("%d ", deliver_value);
    }
    printf("\ncapacity = %zd", stk1.capacity);

    StackDestroy(&stk1);
    */
    return 0;
}
