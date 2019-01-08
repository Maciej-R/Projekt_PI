#include "Adduser.h"

extern std::vector<User> Users_Global;
using std::string;
using std::vector;

string adduser(char* command, int end) {

	vector<string> options(std::move(separate_options(command, end))); //Parse options
	if (options.size() < 2) throw bad_format{"Not enought arguments"};

	auto idx = find_idx(options, "name");							   //User name options index
	string name(option_value(*idx));

	if (std::find_if(Users_Global.cbegin(), Users_Global.cend(),
		[&](const User& usr) { return usr.name() == name; }) != Users_Global.cend()) {
		return string{}; //User already exists
	}

	idx = find_idx(options, "password");							   //User password options index
	string password(option_value(*idx));
	//unsigned char* pass_hash = hash(const_cast<char*>(password.c_str()));
	//password =  std::move(string(pass_hash, pass_hash + 32));

	{
		std::mutex mtx;
		std::lock_guard<std::mutex> guard(mtx);

		Users_Global.emplace_back(name, password);
		auto p = Users_Global[0].name();
		
	}

	//delete pass_hash;

	return name;

}