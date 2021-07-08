#include "test.h"

void printNum(int n)
{
    if(n < 0) return;
    while(n > 0)
    {
        printf("%d\n",n);
        n--;
    }
}

void printNum1(int n)
{
    if(n < 0) return;
    int i = 1;
    while(i <= n)
    {
        printf("%d\n",i);
        i++;
    }
}

