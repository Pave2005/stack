#ifndef CD
#define CD

const size_t coeff = 2;
typedef int Elem_t;

typedef unsigned long long Canary_t;
typedef long long Hash_t;

struct stack
{
    Canary_t l_canary;
    Elem_t* data;
    size_t size;
    size_t capacity;
    unsigned long long status;
    Hash_t stack_hash;
    char* file_name;
    char* func_name;
    char* stack_name;
    Canary_t r_canary;
};

void StackCtor (stack* stk, size_t capacity);
void StackDtor (stack* stk);
void StackRealloc (stack* stk);
void StackPush (stack* stk, Elem_t value);
void StackPop (stack* stk, Elem_t* re_value);
void StackDump (stack* stk);

#endif
