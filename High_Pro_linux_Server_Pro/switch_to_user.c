#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static bool switch_to_user(uid_t user_id,gid_t gp_id)
{
	/*先确保目标用户不是root*/
	if((0 == user_id) && (0 == gp_id))
	{
		return false;
	}
	/*确保当前用户是合法用户：root或者目标用户*/
	gid_t gid = getgid();
	uid_t uid = getuid();
	if( ((0 != gid)||(0 != uid)) && ((gid != gp_id)||(uid != user_id)))
	{
		return false;
	}
	/*如果不是root，则已经是目标用户*/
	if(0 != uid)
	{
		return true;
	}
	
	/*切换到目标用户*/
	if( (setgid(gp_id)<0) || (setuid(user_id) < 0))
	{
		return false;
	}

	return true;
}

int main(int argc,char * argv[])
{
	return 0;
}
