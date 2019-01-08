#include "User.h"

bool operator==(User& u1, User& u2) { return u1.ID() == u2.ID(); }

//unsigned long User::count;

//Duplicate control provided by adduser function
User::User(std::string l, std::string ps) : identifier{0} {

	if (l.size() > 20) throw std::range_error("Login too long");	//Leaves one for /0
	if (ps.size() > 32) throw std::range_error("Password too long");
	strcpy_s(login, l.c_str());
	strcpy_s(password_, ps.c_str());
	
	std::string pth = (fs::current_path().append("users").append(l)).string();
	//No duplicates allowed if exsists must have been created by other source
	if (!fs::exists(pth)) fs::create_directory(pth); 
	strcpy_s(home, pth.c_str());

}
std::string User::name() const { return { login }; }
std::string User::password_change() { return password_; }
const std::string User::password() const { return std::string(password_); }
const fs::path User::home_path() { return home; }	
unsigned long User::ID() const { return identifier; }
//void User::count_check() { count = Users_Global.size(); }
