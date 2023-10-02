#ifndef CD
#define CD

#define StackCtor(stk, max_len) StackCtor_(stk, max_len, #stk, __FILE__, __LINE__, __FUNCTION__);

const size_t coeff = 2;
typedef int Elem_t;
const int POISON = -1;

enum ERRORS
{
    HASH_ERR = 0,
    CANARY_ERR = 1,
    LEN_ERR = 2,
    DATA_ERR = 3,
    REDATA_ERR = 4,
};

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
    const char* file_name;
    const char* var_name;
    const char* func_name;
    int line_num;
    char* stack_name;
    Canary_t r_canary;
};

void StackCtor_ (stack* stk, size_t capacity, const char* var_name,
                 const char* file_name, int line_num, const char* func_name);
void StackDtor (stack* stk);
void StackRealloc (stack* stk);
void StackPush (stack* stk, Elem_t value);
void StackPop (stack* stk, Elem_t* re_value);
void StackDump (stack* stk);

#endif
