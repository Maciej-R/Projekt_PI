#include "Find_User.h"

extern std::vector<User> Userss_Global;

std::vector<User>::iterator find_user(std::string& name) noexcept {

	return std::find_if(Users_Global.begin(), Users_Global.end(),
		[&](User& usr) {return name == usr.name(); });

}