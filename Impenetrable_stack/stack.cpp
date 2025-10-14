#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

StackErr_t StackInit(struct stack_t *stk, ssize_t capacity)
{
    assert(stk != NULL);
    assert(capacity > 0);

    stk->data = (used_type *)calloc((size_t)capacity + 2, sizeof(used_type));
    stk->size = 0;
    stk->capacity = capacity;

    StackErr_t err = NO_ERROR;
    if ((err = StackVerify(stk)))
    {
        STACK_DUMP(stk);
        return err;
    }

    stk->data[0] = CANARY;
    stk->data[(size_t)capacity + 1] = CANARY;
    stk->data++;

    for (ssize_t i = 0; i < capacity; i++)
    {
        stk->data[i] = POIZON;
    }

    err = NO_ERROR;
    if ((err = StackVerify(stk)))
    {
        STACK_DUMP(stk);
        return err;
    }

    return err;
}

StackErr_t StackPush(struct stack_t *stk, used_type value)
{
    ASSERTS(stk);

    StackErr_t err = NO_ERROR;
    if ((err = StackVerify(stk)))
    {
        STACK_DUMP(stk);
        return err;
    }

    if (stk->size == stk->capacity)
    {
        STACK_DUMP(stk);
        return err;
    } 

    stk->data[stk->size++] = value;
    printf("%d - %zd\n", value, stk->size - 1);
    
    stk->sum_of_elements += value;

    if (stk->size >= stk->capacity)
    {
        printf("MORE\n");
        stk->data = (used_type *)realloc(--stk->data, 2*(size_t)(stk->capacity + 1)*sizeof(used_type));
        stk->data++;
        stk->data[(size_t)stk->capacity] = POIZON;
        
        err = NO_ERROR;
        if ((err = StackVerify(stk)))
        {
            STACK_DUMP(stk);
            return err;
        }
        stk->capacity *= INCREASE_IN;
        stk->data[(size_t)stk->capacity] = CANARY;
    }
    
    for (ssize_t j = stk->size; j < stk->capacity; j++)
    {
        stk->data[j] = POIZON;
    }

    err = NO_ERROR;
    if ((err = StackVerify(stk)))
    {
        STACK_DUMP(stk);
        return err;
    }

    ASSERTS(stk);

    return err;
}

used_type StackPop(struct stack_t *stk, StackErr_t *err)
{
    ASSERTS(stk);

    *err = StackVerify(stk);
    if (*err) return POIZON;
    if (stk->data[--stk->size] == CANARY)
    {
        STACK_DUMP(stk);
        return POIZON;
    }

    used_type value_from_stack = stk->data[stk->size];
    stk->data[stk->size] = POIZON;

    if (stk->size < 0) stk->size = 0;

    *err = StackVerify(stk);
    if (*err) return POIZON;

    printf("%d - ", value_from_stack);
    printf("%zd\n", stk->size);

    return value_from_stack;
}

StackErr_t StackDestroy(struct stack_t *stk)
{
    ASSERTS(stk);

    StackErr_t err = NO_ERROR;
    if ((err = StackVerify(stk)))
    {
        STACK_DUMP(stk);
        return err;
    }

    free(--stk->data);
    stk->data = NULL;
    stk = NULL;

    return err;
}

StackErr_t StackVerify(const struct stack_t *stk)
{
    ASSERTS(stk);

    if (stk->size < 0 || stk->size > stk->capacity) return ERROR_IN_SIZE;
    else if (stk->capacity <= 0) return ERROR_IN_CAPACITY;
    else if (stk->data == NULL) return ERROR_IN_DATA;
    return NO_ERROR;
}

void StackDump(const struct stack_t *stk, const char *file, int line)
{
    printf("StackDump called from %s :%d\n", file, line);
    printf("Stack[%p]\n", &stk);
    printf("{\n");
    printf("    size = %ld\n", stk->size);
    printf("    capacity = %ld\n", stk->capacity);
    printf("    data[%p]\n", stk->data);
    printf("{\n");
    for (ssize_t i = 0; i < stk->capacity; i++)
    {
        printf("    [%zd] = %d\n", i, stk->data[i]);
    }
    printf("    }\n");
    printf("}\n");
}