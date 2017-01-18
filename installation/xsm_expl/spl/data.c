#include "data.h"

int isAllowedRegister(int value)
{
    if(value>=R0&&value<R0+C_REG_BASE)
        return 1;
    return 0;
}
