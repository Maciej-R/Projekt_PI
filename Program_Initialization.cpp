#include "Program_Initialization.h"
#include <fstream>

using std::vector;
extern vector<User> Users_Global;
extern vector<Group> Groups_Global;
extern std::string server_main_directory;

void Initialize::read_data() {			//Reads basic program data

	std::ifstream in("Users_Global.data");
	
	unsigned long length;
	in >> length;	//if 0 return?
	
	auto p = length / sizeof(User);
	Users_Global.reserve(p);

	while (p-- > 0) {
	
		User* usr = new User{};			//Memory for temporary object
		const int sz{ sizeof(User) };
		char buf[sz];
		in.read(buf, sz);
		memcpy(usr, buf, sz);			//Overrides object data with input
		Users_Global.emplace_back(*usr); //Adds to working vector
	
	}
	
	in.close();
	//p = Users_Global.size();
		
	in.open("Groups_Global.data");
	read_groups(Groups_Global, in);
	in.close();

	in.open("main_directory.server");
	in >> server_main_directory;
	in.close();

}

void Initialize::save_data() {			//Writes basic program data

	std::ofstream of("Users_Global.data", std::ofstream::trunc);    //Clear and write in binary form (override previous data)
	of.close();
	of.open("Users_Global.data", std::ofstream::app);

	unsigned long data_to_write = Users_Global.size() * sizeof(User);						//Represents length of table
	of << data_to_write;
	char * ptr = reinterpret_cast<char*>(Users_Global.data());								//Write data from table
	of.write(ptr, data_to_write);
	of.close();

	of.open("Groups_Global.data", std::ofstream::trunc);
	of.close();
	of.open("Groups_Global.data", std::ofstream::app);
	
	data_to_write = Groups_Global.size() * sizeof(Group);						//Represents length of table in bytes
	of << data_to_write;
	of << '\n';

	//data_to_write = Groups_Global.size();										//Number of groups
	//of << data_to_write;

	ptr = reinterpret_cast<char*>(Groups_Global.data());								//Write object data
	of.write(ptr, data_to_write);
	of << '\n';
	
	for (auto x : Groups_Global) {
	
		save_group(x, of);		//Writes vectors content
	
	}
	//ptr = reinterpret_cast<char*>(Groups_Global.data());
	//of.write(ptr, data_to_write);
	of.close();

	of.open("main_directory.server");
	of << server_main_directory;
	of.close();

}

bool Initialize::check_requirements() {

	bool correct = true;
	std::ifstream in("Users_Global.data", std::ifstream::binary);

	if (!in) {											//Checks whether file exists

		std::ofstream of("Users_Global.data");			//In case not creates
		of << 0;										//And marks 0 data length
		of.close();

		of.open("main_directory.server");
		of << server_main_directory;
		of.close();


	}in.close();

	in.open("Groups_Global.data", std::ifstream::binary);

	if (!in) {											//Checks whether file exists

		std::ofstream of("Groups_Global.data");
		of << 0;
		of.close();


	}in.close();

	fs::path groups_path = fs::current_path().append("groups");
	if (!fs::exists(groups_path)) {
	
		fs::create_directory(groups_path);

	}

	fs::path users_path = fs::current_path().append("users");
	if (!fs::exists(users_path)) {

		fs::create_directory(users_path);

	}

	fs::path temps_path = fs::current_path().append("temporary");
	if (!fs::exists(temps_path)) {

		fs::create_directory(temps_path);

	}

	return correct;

}

void Initialize::post_init(){

	remap_membership();
	//User::count_check();	
	//Group::count_check();

}

/*void Initialize::start_server() {



}*/

/*char * ptr = reinterpret_cast<char*>(Users_Global.data());
	in.read(ptr, length);*/