#include <stdio.h>
#include <stdlib.h>
#include "stack_cd.h"
#include "stack_protect.h"

// &stk
// "&stk"
//#define StackCtor(stk, max_len) StackCtor_(stk, max_len, #stk, __FILE__, __LINE__, __FUNCTION__);

// void StackCtor_(stack* stk, size_t max_len, const char* var_name,
//                 const char* file_name, int line_num, const char* func_name)

void StackCtor(stack* stk, size_t max_len)
{
    size_t canary_size = sizeof (Canary_t) / sizeof (Elem_t);
    stk->status = 0;

    if (max_len < 0)
    {
        stk->status |= (1 << 2);
        return;
    }

    stk->data = (Elem_t*)calloc (max_len + 2 * canary_size, sizeof(Elem_t));
    if (stk->data == NULL)
    {
        stk->status |= (1 << 3);
        return;
    }

    stk->l_canary = 0xBADC;
    stk->r_canary = 0xBADC;

    *(Canary_t*)stk->data = stk->l_canary;

    //stk->data = (Elem_t*)((char*)stk->data + sizeof(Canary_t));

    *(Canary_t*)(stk->data + sizeof (Canary_t) + max_len * sizeof (Elem_t))  = stk->l_canary;

    stk->size = 0;
    stk->capacity = max_len;
    stk->stack_hash = Hash (stk);
    //stk->file_name = file_name;
    //stk->func_name = __func__;
    //stk->stack_name = #stack;
}

void StackDtor (stack* stk)
{
    free (stk->data);
    // POISON
    stk->capacity = -1;
    stk->size = -1;
    stk->r_canary = -1;
    stk->l_canary = -1;
    stk->stack_hash = -1;
    stk->status = -1;
}

void StackRealloc (stack* stk)
{
    stk->capacity *= coeff;
    size_t re_size = stk->capacity * sizeof (Elem_t) + 2 * sizeof (Canary_t);
    stk->data = (Elem_t*)realloc (stk->data , re_size);
    *(Canary_t*)(stk->data + sizeof (Canary_t) + stk->capacity * sizeof (Elem_t)) = stk->l_canary;
    if (stk->data == NULL)
        stk->status |= (1 << 3);
}

void StackPush (stack* stk, Elem_t value)
{
    HashProtection (stk);
    CanaryProtection (stk);
    if (stk->size >= stk->capacity)
        StackRealloc (stk);
    *((Elem_t*)((char*)stk->data + sizeof (Canary_t)) + stk->size++) = value;
    stk->stack_hash = ReHash (stk);
}

void StackPop (stack* stk, Elem_t* re_value)
{
    HashProtection (stk);
    CanaryProtection (stk);
    stk->size--;
    //*re_value = ((char*)stk->data +);
    ReHash (stk);
}

void StackDump (stack* stk)
{
    //printf ("File - %s\n", info->file_name);
    //printf ("Function - %s\n", info->func_name);
    //printf ("Stack name - %s", info->stack_name);
    printf ("%llu\n\n", stk->status);
    printf ("size - %lu\n", stk->size);
    printf ("capacity of stack - %lu\n\n", stk->capacity);
    for (size_t i = 0; i < stk->capacity; i++)
    {
        if (*((Elem_t*)((char*)stk->data + sizeof (Canary_t)) + i) != 0)
            printf ("*[%lu] = %d\n", i + 1, *((Elem_t*)((char*)stk->data + sizeof (Canary_t)) + i));
        else
            printf (" [%lu] = %d\n", i + 1, *((Elem_t*)((char*)stk->data + sizeof (Canary_t)) + i));
    }
}
