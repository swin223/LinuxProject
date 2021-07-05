#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int a = 10;
    int b[] = {1,2,3,4};
    char c[] = "Hello,Swin";
    int *d = (int *)malloc(a*sizeof(int));
    if(d == NULL)
    {
        printf("malloc error\n");
        return -1;
    }

    for(int i = 0;i < 10;i++)
    {
        d[i] = i;
    }

    free(d);
    d = NULL;
    float e = 8.5f;
    return 0;
}

