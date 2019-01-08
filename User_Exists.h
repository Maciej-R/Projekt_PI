#pragma once
#include "User.h"
#include <vector>

extern std::vector<User> Users_Global;

bool user_exists(std::string&);
bool user_exists(unsigned long);