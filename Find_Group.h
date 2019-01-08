#pragma once
#include "Group.h"
#include <vector>
#include <string>
#include <algorithm>

class Group;

std::vector<Group>::iterator find_group(std::string&) noexcept;  //Finds iterator to group by name