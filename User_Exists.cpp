#include "User_Exists.h"

bool user_exists(std::string& name) {

	for (auto x : Users_Global) if (x.name() == name) return true;

	return false;

}

bool user_exists(unsigned long id) {

	for (auto x : Users_Global) if (x.ID() == id) return true;

	return false;

}