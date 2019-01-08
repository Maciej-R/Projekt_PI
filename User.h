#ifndef MYUSER
#define MYUSER

#pragma once
#include <string>
#include <filesystem>
#include <algorithm>
#include <exception>

namespace fs = std::experimental::filesystem;
class User;
extern std::vector<User> Users_Global;

class User {

private:

	unsigned long identifier;
	//static unsigned long count;
	//DO NOT change for string 
	//Handle to resources casuses pointer to value save during save
	//Any size change causes save incompatibility
	char login[21];
	char password_[33];
	char home[255];	//Max on Linux, Win 260
	//fs::path home;


public:

	User(std::string l, std::string ps);
	User() {}

	std::string name() const;
	std::string password_change();
	const std::string password() const;
	const fs::path home_path();
	unsigned long ID() const;
	//static void count_check();
	//static unsigned long total() { return count; }
	const char* cpassword () const { return password_; }

};

namespace std {
	template<>
	struct less<User> {
		bool operator()(const User& u1, const User& u2) const { return u1.ID() < u2.ID(); }
	};
}

#endif