#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack_cd.h"

int main ()
{
    stack stk = {};
    size_t ret = StackCtor (&stk, -2);
    if (ret == OK)
    {
    StackPush (&stk, 20);
    StackPush (&stk, 2);
    StackPush (&stk, 5);
    Elem_t tmp = 0;
    StackPop (&stk, &tmp);
    StackDump (&stk);
    //printf ("%d", tmp);
    StackDtor (&stk);
    }
}
