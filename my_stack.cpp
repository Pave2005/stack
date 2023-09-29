#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack_cd.h"

//#define STACK_CTOR (stack) StackCtor ((stack), #stack, __LINE__, __FILE__, __fun__)

int main ()
{
    printf("OK!\n");
    stack stk = {};
    StackCtor (&stk, -10);
    printf("stk.status = %b\n", stk.status);

    StackPush (&stk, 20);
    Elem_t tmp = 0;
    StackPop (&stk, &tmp);
    StackDump (&stk);
    printf ("\n%d", tmp);
    StackDtor (&stk);
}
