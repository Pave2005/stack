#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack_cd.h"

int main ()
{
    stack stk = {};
    StackCtor (&stk, 5);
    StackPush (&stk, 20);
    Elem_t tmp = 0;
    StackPop (&stk, &tmp);
    StackDump (&stk);
    printf ("%d", tmp);
    StackDtor (&stk);
}
