#include <stdio.h>
#include <stdlib.h>
#include "stack_cd.h"
#include "stack_protect.h"

// &stk
// "&stk"

void StackCtor_(stack* stk, size_t max_len, const char* var_name,
                const char* file_name, int line_num, const char* func_name)
{
    size_t canary_size = sizeof (Canary_t) / sizeof (Elem_t);
    stk->status = 0;

    if (max_len < 0)
    {
        stk->status |= (1 << LEN_ERR);
        return;
    }

    stk->data = (Elem_t*)calloc (max_len + 2 * canary_size, sizeof(Elem_t));
    if (stk->data == NULL)
    {
        stk->status |= (1 << DATA_ERR);
        return;
    }

    stk->l_canary = 0xBADC;
    stk->r_canary = 0xBADC;

    *(Canary_t*)stk->data = stk->l_canary;

    stk->data = (Elem_t*)((char*)stk->data + sizeof(Canary_t)); // перемеслил указатель начала мессива через канарейку

    *(Canary_t*)(stk->data + max_len * sizeof (Elem_t)) = stk->l_canary;

    stk->size = 0;
    stk->capacity = max_len;
    stk->stack_hash = Hash (stk);
    stk->var_name = var_name;
    stk->file_name = file_name;
    stk->line_num = line_num;
    stk->func_name = func_name;
    //stk->stack_name = #stack;
}

void StackDtor (stack* stk)
{
    //free (stk->data);
    // POISON
    stk->capacity = POISON;
    stk->size = POISON;
    stk->r_canary = POISON;
    stk->l_canary = POISON;
    stk->stack_hash = POISON;
    stk->status = POISON;
}

void StackRealloc (stack* stk)
{
    stk->capacity *= coeff;
    size_t re_size = stk->capacity * sizeof (Elem_t) + 2 * sizeof (Canary_t);

    stk->data = (Elem_t*)((char*)stk->data - sizeof (Canary_t));
    stk->data = (Elem_t*)realloc (stk->data , re_size);
    stk->data = (Elem_t*)((char*)stk->data + sizeof (Canary_t));

    *(Canary_t*)(stk->data + stk->capacity * sizeof (Elem_t)) = stk->l_canary;
    if (stk->data == NULL)
        stk->status |= (1 << REDATA_ERR);
}

void StackPush (stack* stk, Elem_t value)
{
    HashProtection (stk);
    CanaryProtection (stk);
    if (stk->size >= stk->capacity)
        StackRealloc (stk);
    *(stk->data + stk->size++) = value;
    stk->stack_hash = ReHash (stk);
}

void StackPop (stack* stk, Elem_t* re_value)
{
    HashProtection (stk);
    CanaryProtection (stk);
    stk->size--;
    *re_value = *(stk->data + stk->size);
    ReHash (stk);
}

void StackDump (stack* stk)
{
    printf ("File - %s\n", stk->file_name);
    printf ("Function - %s\n", stk->func_name);
    printf ("Stack name - %s\n", stk->var_name);
    printf ("Line - %d\n", stk->line_num);
    printf ("%llu\n\n", stk->status);
    printf ("size - %lu\n", stk->size);
    printf ("capacity of stack - %lu\n\n", stk->capacity);
    for (size_t i = 0; i < stk->capacity; i++)
    {
        if (*(stk->data + i) != 0)
            printf ("*[%lu] = %d\n", i + 1, *(stk->data + i));
        else
            printf (" [%lu] = %d\n", i + 1, *(stk->data + i));
    }
}
