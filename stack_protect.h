#ifndef PROTECT
#define PROTECT

void CanaryProtection (stack* stk);
unsigned long long Hash (stack* stk);
void HashProtection (stack* stk);
unsigned long long ReHash (stack* stk);

#endif
