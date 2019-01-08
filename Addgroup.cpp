#include "Addgroup.h"

extern vector<Group> Groups_Global;
class Separation;

//Creates group by default owned by current user
//Or specified by -owner option 
//Option -wm will prevent adding current user as owner

bool addgroup(Result& res, User& usr) {

	//if (res.options.size() < 1) throw bad_format(string{ "Not enought arguments" });
	if (res.options.find("name") == res.options.end()) throw bad_format(string{ "Name needed" });
	
	string& name = res.options.at("name");

	if (std::find_if(Groups_Global.cbegin(), Groups_Global.cend(),
		[&](const Group& gr) { return gr.name() == name; }) != Groups_Global.cend()) {
		return false; //Group already exists
	}

	{
		std::mutex mtx;
		std::lock_guard<std::mutex> guard(mtx);
		bool b = res.options.find("private") != res.options.end();

		Groups_Global.emplace_back(name, b);		//Creation

	}

	vector<Group>::iterator grp = std::find_if(Groups_Global.begin(), Groups_Global.end(),
		[&](Group& g) {return name == g.name(); });

	if (res.options.find("owner") != res.options.end()) {

		string& owner = res.options.at("owner");
		vector<string> names(std::move(separate_options(owner.c_str(), owner.length(), [](char c)->bool { return std::isspace(c); })));
		for (auto it = names.begin(); it != names.end(); ++it) {		//Option is -owner ow1 ow2 ow3
		
			if(user_exists(*it)) grp->addowner(*find_user(*it));
			/*else {
			
				string msg = "Unable to add owner: " + uname;
				char buf[50];
				strcpy_s(buf, msg.c_str());
				send_to_host()
			
			}*/
		
		}
		
	}
	else {
	
		string owner = usr.name();
		grp->addowner(*find_user(owner));

	}
	if (res.options.find("wm") != res.options.end() && !grp->owns(usr.name()) || grp->owners_count() == 0) grp->addowner(usr); //wm option for without me

	return true;

}

/*

vector<string> options(std::move(separate_options(command, end))); //Parse options
	if (options.size() < 1) throw bad_format(string{ "Not enought arguments" });

	vector<string>::iterator idx = find_idx(options, "name");							   //Group name options index
	string name(option_value(*idx));

	if (std::find_if(Groups_Global.cbegin(), Groups_Global.cend(),
		[&](const Group& gr) { return gr.name() == name; }) != Groups_Global.cend()) {
		return false; //Group already exists
	}

	{
		std::mutex mtx;
		std::lock_guard<std::mutex> guard(mtx);
		bool b = find_idx(options, "private") != options.end();

		Groups_Global.emplace_back(name, b);		//Creation

	}

	vector<Group>::iterator grp = std::find_if(Groups_Global.begin(), Groups_Global.end(),
		[&](Group& g) {return name == g.name(); });

	string owner;
	idx = find_idx(options, "owner");
	if (idx != options.end()) {

		vector<string> names(std::move(separate_options(idx->c_str() + 5, idx->length() - 5, [](char c)->bool { return std::isspace(c); })));
		for (auto it = names.begin(); it != names.end(); ++it) {		//Option is -owner ow1 ow2 ow3

			if(user_exists(*it)) grp->addowner(*find_user(*it));
			else {

				string msg = "Unable to add owner: " + uname;
				char buf[50];
				strcpy_s(buf, msg.c_str());
				send_to_host()

			}

		}

	}
	else {

	owner = usr.name();
	grp->addowner(*find_user(owner));

	}
	if (find_idx(options, "wm") != options.end() && !grp->owns(usr.name())) grp->addowner(usr); //wm option for without me

	return true;

*/