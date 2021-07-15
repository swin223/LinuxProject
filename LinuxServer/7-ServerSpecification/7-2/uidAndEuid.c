#include <unistd.h>
#include <stdio.h>

int main()
{
    uid_t uid = getuid();
    uid_t euid = geteuid();
    printf("uid id %d,euid is %d",uid,euid);
    return 0;
}
