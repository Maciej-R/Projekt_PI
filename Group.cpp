#include "Group.h"

//unsigned long Group::count;

Group::Group(std::string n, bool p) : isPrivate{ p }, identifier{0} {

	strcpy_s(name_, n.c_str());
	std::string path = fs::current_path().append("groups").append(name_).string();
	strcpy_s(home_directory, path.c_str());								//Sets home directory to main_path/group_name
	fs::create_directory(home_directory);

}

/*Group::Group(const Group& grp){

	identifier = grp.ID();
	strcpy_s(home_directory, grp.home_directory);
	strcpy_s(name_, grp.name_);
	isPrivate = grp.isPrivate;
	std::copy(grp.owners.begin(), grp.owners.end(), std::back_inserter(owners));
	std::copy(grp.members.begin(), grp.members.end(), std::back_inserter(members));
	
}*/

std::string Group::name() const { return name_; }

void Group::addmember(User& u) { members.push_back(u); }

void Group::addowner(User& u) { owners.push_back(u); }

int Group::members_count() const { return members.size(); }

void Group::make_public() { isPrivate = false; }

void Group::make_private() { isPrivate = true; }

unsigned long Group::ID() const { return identifier; }

fs::path Group::home() const { return { std::string(home_directory) }; }

size_t Group::owners_count() const { return owners.size(); }

//void Group::count_check() { count = Groups_Global.size(); }

bool Group::owns(std::string& name) const {

	if (std::find_if(owners.begin(), owners.end(), [&](const User& usr) { return usr.name() == name; }) != owners.end()) return true;
	return false;

}

bool Group::owns(std::string name) const {

	if (std::find_if(owners.begin(), owners.end(), [&](const User& usr) { return usr.name() == name; }) != owners.end()) return true;
	return false;

}

void save_group(Group& g, std::ofstream& of) {

	size_t members_data = sizeof(User) * g.members.size();
	size_t owners_data = sizeof(User) * g.owners.size();
	/*unsigned long data_size = sizeof(Group);// +members_data + owners_data + 2 * sizeof(size_t) + sizeof(unsigned long);
	of << data_size;*/
	
	size_t count = g.owners.size();
	of << count;
	of << '\n';
	char* ptr = reinterpret_cast<char*>(g.owners.data());
	of.write(ptr, owners_data);
	of << '\n';
	

	count = g.members.size();
	of << count;
	of << '\n';
	ptr = reinterpret_cast<char*>(g.members.data());
	of.write(ptr, members_data);
	of << '\n';
	
}

/*void save_groups(vector<Group>&, std::ofstream&) {



}*/

void read_groups(std::vector<Group>& vg, std::ifstream& in) {

	unsigned long data_size;			//Total size of object data
	in >> data_size;
	in.ignore(1, '\n');

	if (data_size < sizeof(Group)) return;

	unsigned long groups_to_read = data_size / sizeof(Group);		//Group count

	vg.clear();
	size_t create = groups_to_read;
	{
		//const int sz{ sizeof(Group) };
		for (unsigned int i = 0; i < groups_to_read; ++i) {
		
			vg.push_back(Group());
		
		}
		void* p = malloc(data_size);
		in.read(static_cast<char*>(p), data_size);
		memcpy(vg.data(), static_cast<char*>(p), data_size);
		in.ignore(1, '\n');
		//std::uninitialized_copy(static_cast<Group*>(p), static_cast<Group*>(p) + groups_to_read, vg.data());
		//std::vector<User> Group::*owners_ptr = &Group::owners;
		//std::vector<User> Group::*members_ptr = &Group::members;
		/*for (int i = 0; create--; i++) {

			std::vector<User>* tmp = new std::vector<User>();
			memcpy(reinterpret_cast<void*>(&(vg[i].owners)), reinterpret_cast<char*>(tmp), sizeof(std::vector<User>));
			std::vector<User>* tmp2 = new std::vector<User>();
			memcpy(reinterpret_cast<void*>(&(vg[i].members)), reinterpret_cast<char*>(tmp2), sizeof(std::vector<User>));
			int x = 0;
			Group grp{};			//Memory for temporary object
			char buf[sz];
			vg.push_back(grp);
			in.read(buf, sz);
			memcpy(&grp, buf, sz);				//Overrides object data with input
			//grp.members = std::vector<User>();		//Pointers clearing
			//grp.owners.clear();
			vg.at(i) = grp;			//Adds to working vector

		}*/

	}
	for (unsigned int i = 0; i < groups_to_read; ++i) {
		
		//in >> data_size;
		//in.ignore(5);
		//in.read(reinterpret_cast<char*>(&data_size), sizeof(size_t));
		//in.read(reinterpret_cast<char*>(&data_size), sizeof(size_t));			//For length of one group record
		//std::vector<User> res;
		size_t owners_in_group;// = data_size / sizeof(User);
		in >> owners_in_group;
		in.ignore(1, '\n');

		std::vector<User>* tmp = new std::vector<User>();
		memcpy(reinterpret_cast<void*>(&(vg[i].owners)), reinterpret_cast<char*>(tmp), sizeof(std::vector<User>));

		//in >> owners_in_group;
		while (owners_in_group-- > 0) {
		
			User* usr = new User{};			//Memory for temporary object
			const int sz{ sizeof(User) };
			char buf[sz];
			in.read(buf, sz);
			memcpy(usr, buf, sz);			//Overrides object data with input
			vg[i].owners.push_back(*usr);			//Adds to working vector
		
		}

		//memcpy(vg[i].owners.data(), res.data(), data_size);
		//vg[i].owners = std::move(res);

		//res.clear();
		//in.read(reinterpret_cast<char*>(&data_size), sizeof(size_t));
		//in >> data_size;
		in.ignore(1, '\n');
		size_t members_in_group;// = data_size / sizeof(User);
		in >> members_in_group;
		
		std::vector<User>* tmp2 = new std::vector<User>();
		memcpy(reinterpret_cast<void*>(&(vg[i].members)), reinterpret_cast<char*>(tmp2), sizeof(std::vector<User>));
	
		//in >> members_in_group;
		while (members_in_group-- > 0) {

			User* usr = new User{};			//Memory for temporary object
			const int sz{ sizeof(User) };
			char buf[sz];
			in.read(buf, sz);
			memcpy(usr, buf, sz);			//Overrides object data with input
			vg[i].members.push_back(*usr);			//Adds to working vector

		}

		//vg[i].members = std::move(res);
	
	}

}