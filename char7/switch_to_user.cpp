
static bool switch_to_user(uid_t user_id, gid_t gp_id)
{
	if (( user_id == 0 ) && ( gp_id ==0 )) {
			return false;
	}

	git_t gid = getgid();
	uid_t uid = getuid();
	
	if ((( gid !=0 ) || ( uid != 0 )) && (( gid != gp_id) || ( uid != user_id ))) {
		return false;
	
