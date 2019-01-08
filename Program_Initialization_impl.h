#pragma once
#include "Program_Initialization.h"
#include "Group.h"
#include <fstream>

using std::vector;

void Initialize::read_data() {			//Reads basic program data

	std::ifstream in("Users_Global.data", std::ifstream::binary);
	unsigned long length;
	in >> length;
	extern vector<User> Users_Global;
	Users_Global.reserve(length / sizeof(User));
	char * ptr = reinterpret_cast<char*>(Users_Global.data());
	in.read(ptr, length);
	in.close();
	in.open("Groups_Global.data");
	in >> length;
	extern vector<Group> Groups_Global;
	Groups_Global.reserve(length / sizeof(User));
	ptr = reinterpret_cast<char*>(Groups_Global.data());
	in.read(ptr, length);
	in.close();

}

void Initialize::save_data() {			//Writes basic program data

	std::ofstream of("Users_Global.data", std::ifstream::binary | std::ofstream::trunc);    //Clear and write in binary form (override previous data)
	extern vector<User> Users_Global;
	unsigned long data_to_write = Users_Global.size() * sizeof(User);
	of << data_to_write;
	char * ptr = reinterpret_cast<char*>(Users_Global.data());
	of.write(ptr, data_to_write);
	of.close();
	extern vector<Group> Groups_Global;
	data_to_write = Groups_Global.size() * sizeof(User);
	of << data_to_write;
	ptr = reinterpret_cast<char*>(Groups_Global.data());
	of.write(ptr, data_to_write);
	of.close();

}

void Initialize::start_server() {



}