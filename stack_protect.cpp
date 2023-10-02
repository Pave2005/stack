#include <stdio.h>
#include "stack_cd.h"
#include "stack_protect.h"

unsigned long long Hash (stack* stk)
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    unsigned int k = 0;
    const int r = 18;
    unsigned char* elem = (unsigned char*)stk;
    unsigned long long len = sizeof (stk);
    unsigned long long h = seed ^ len;
    while (len >= 4)
    {
        k  = elem[0];
        k |= elem[1] << 8;
        k |= elem[2] << 16;
        k |= elem[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        elem += 4;
        len -= 4;
    }
    switch (len)
    {
        case 3:
        h ^= elem[2] << 16;
        case 2:
        h ^= elem[1] << 8;
        case 1:
        h ^= elem[0];
        h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}
void HashProtection (stack* stk)
{
    unsigned long long hash_before = stk->stack_hash;
    unsigned long long hash_now = ReHash (stk);
    if (hash_now != hash_before)
    {
        stk->status |= (1 << HASH_ERR);
    }
}

unsigned long long ReHash (stack* stk)
{
    stk->stack_hash = 0;
    return Hash (stk);
}

void CanaryProtection (stack* stk)
{
    if (stk->l_canary != 0xBADC && stk->r_canary != 0xBADC && stk->data[0] != 0xBADC && stk->data[stk->capacity + 1] != 0xBADC)
    {
        stk->status |= (1 << CANARY_ERR);
    }
}

// 000001110
//    |
// 000000010
//     &
// 000001111
//    =
// 000000010

// status & ( HASH_ERROR ) == HASH_ERROR;
// 000000000

// 000000001
// 000001111
// 000000001
// 3 << 1;
// | &


