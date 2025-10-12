#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stddef.h>

#define POIZON 765418
#define STACK_DUMP(stk)\
        StackDump(stk,__FILE__, __LINE__)
#define INCREASE_IN 2
#define CANARY 43685
#define MAX_VALUE __INT_MAX__

#define ASSERTS(stk) \
    assert(stk != NULL);\
    assert(stk->size >= 0);\
    assert(stk->capacity > 0);\
    assert(stk->data != NULL);


typedef int used_type;

enum StackErr_t {
    NO_ERROR = 0,
    ERROR_IN_SIZE,
    ERROR_IN_CAPACITY,
    ERROR_IN_DATA,
    ERROR_IN_DATA_INTEGRITY,
};

struct stack_t
{
    used_type *data;
    ssize_t size;
    ssize_t capacity; 
    ssize_t sum_of_elements;
};

StackErr_t StackInit(struct stack_t *stk, ssize_t capacity);
StackErr_t StackPush(struct stack_t *stk, used_type value);
used_type StackPop(struct stack_t *stk, StackErr_t *err);
StackErr_t StackDestroy(struct stack_t *stk);
StackErr_t StackVerify(const struct stack_t *stk);
void StackDump(const struct stack_t *stk, const char *file, int line);
StackErr_t HashFunction(const struct stack_t *stk, ssize_t sum_of_elements);

#endif