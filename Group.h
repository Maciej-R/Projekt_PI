#ifndef MYGROUP
#define MYGROUP

#pragma once
#include "User.h"
//#include "Server.h"
#include <set>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <memory>
#include <boost/shared_ptr.hpp>

namespace fs = std::experimental::filesystem;
class Group;
class Connection;
class Result;
extern std::vector<Group> Groups_Global;

void save_group(Group&, std::ofstream&);
//void save_groups(vector<Group>&, std::ofstream&);
void read_groups(std::vector<Group>&, std::ifstream&);

class Group {

private:					

	friend void remap_membership();
	friend void manage_group(User&, boost::shared_ptr<Connection>, Result&);
	friend void save_group(Group&, std::ofstream&);
	friend void read_groups(std::vector<Group>&, std::ifstream&);
	std::vector<User> owners;															
	std::vector<User> members;
	char name_[21];
	char home_directory[255];	//Max on Linux, Win 260
	unsigned long identifier;
	//static unsigned long count;
	bool isPrivate;

public:

	Group(std::string n, bool p = true);
	Group() {}
	//Group(const Group&);// = default;
	std::string name() const;
	void addmember(User& u);
	void addowner(User& u);
	int members_count() const;
	void make_public();
	void make_private();
	unsigned long ID() const;
	fs::path home() const;
	bool owns(std::string&) const;
	bool owns(std::string) const;
	size_t owners_count() const;
	//static void count_check();
	//static unsigned long total() { return count; }
	
};

#endif MYGROUP