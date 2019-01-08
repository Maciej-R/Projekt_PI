#include "Find_Group.h"

extern std::vector<Group> Groups_Global;

std::vector<Group>::iterator find_group(std::string& name) noexcept {

	return std::find_if(Groups_Global.begin(), Groups_Global.end(),
		[&](Group& grp) {return name == grp.name(); });

}