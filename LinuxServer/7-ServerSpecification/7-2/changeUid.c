#include <stdio.h>
#include <unistd.h>

static bool switch_to_user(uid_t user_id,gid_t gp_id)
{
    // 先确保目标用户不是root
    if((user_id == 0) && (gp_id == 0))
    {
        return false;
    }

    // 确保当前用户是合法用户 不是root或者是目标用户
    gid_t gid = getgid();
    uid_t uid = getuid();
    if(((gid != 0) || (uid != 0)) && ((gid != gp_id) || (uid != user_id)))
    {
        return false;
    }

    // 如果不是root就表明已经是目标用户，不需要切换
    if(uid != 0)
    {
        return true;
    }

    // 剩下的情况:要么是root用户，要么就是剩下非目标用户
    if((setgid(gp_id) < 0)||(setuid(user_id) < 0))
    {
        return false;
    }

    return true;
}
