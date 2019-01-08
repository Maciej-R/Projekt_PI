#pragma once
#include "User.h"
#include <vector>
#include <string>
#include <algorithm>

std::vector<User>::iterator find_user(std::string&) noexcept;  //Finds iterator to user by name