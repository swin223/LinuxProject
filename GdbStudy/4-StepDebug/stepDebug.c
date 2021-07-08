#include <stdio.h>

int add(int a,int b)
{
    int c = a + b;
    return c;
}

int count(int num)
{
    int i = 0;
    if(0 > num) return 0;
    while(i < num){
        printf("%d\n",i);
        ++i;
    }
    return i;
}

int main(void)
{
    int a = 3;
    int b = 7;
    printf("it will calc a + b\n");
    int c = add(a,b);
    printf("%d + %d = %d\n",a,b,c);
    count(c);
    return 0;
}

