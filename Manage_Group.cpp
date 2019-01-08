#include "Manage_Group.h"

void deny_access(boost::shared_ptr<Connection> con) {

	char buf[]{ "Access denied!" };
	send_to_host(con->shared_from_this(), boost::asio::buffer(buf, sizeof(buf)));

}

void manage_group(User& usr, boost::shared_ptr<Connection> con, Result& res) {

	if (res.options.find("name") == res.options.end()) {		   //Command format: upload file_name

		char msg[] = { "Unknown group" };
		send_to_host(con->shared_from_this(), buffer(msg, sizeof(msg)));

	}

	vector<Group>::iterator grp;					//Operation target
	string& name = res.options.at("name");
	grp = find_group(name);			//Group name

	if (grp == Groups_Global.end()) return;

	auto idx = res.options.find("addmember");
	auto end = res.options.end();

	bool owns{ grp->owns(usr.name()) };

	if (idx != end) {

		if (!owns) {

			deny_access(con->shared_from_this());
			return;

		}
		vector<User>::iterator user;
		name = idx->second;
		user = find_user(name);

		grp->addmember(*user);

	}
	if ((idx = res.options.find("addowner")) != end) {

		if (!owns) {
		
			deny_access(con->shared_from_this());
			return;
		
		}
		vector<User>::iterator usr;
		name = idx->second;
		usr = find_user(name);

		grp->addowner(*usr);

	}

	if ((idx = res.options.find("private")) != end) {

		if (!owns) {

			deny_access(con->shared_from_this());
			return;

		}
		string val = idx->second;
		if (val == "true") grp->make_private();
		else if (val == "false") grp->make_public();
		else {
			char buf[]{ "Wrong privacy value" };
			send_to_host(con->shared_from_this(), boost::asio::buffer(buf, sizeof(buf)));
		}

	}



	if ((idx = res.options.find("count")) != end) {

		int c = grp->members_count();
		char buf[sizeof(int) + 1];
		strcpy_s(buf, std::to_string(c).c_str());
		send_to_host(con->shared_from_this(), boost::asio::buffer(buf, sizeof(buf)));
	
	}

	if ((idx = res.options.find("home")) != end) {

		string pth = (grp->home()).string();
		char* buf = new char[pth.length()];
		strcpy_s(buf, pth.length(), pth.c_str());
		send_to_host(con->shared_from_this(), boost::asio::buffer(buf, sizeof(buf)));

	}

	if ((idx = res.options.find("ls")) != end) {

		string s;
		if ((idx->second).find("owners") != string::npos) {
		
			for (auto& x : grp->owners) {
			
				s += x.name();
			
			}
		
		}
		
		if ((idx->second).find("members") != string::npos) {

			for (auto& x : grp->members) {

				s += x.name();

			}

		}

		try {
			if (s.length() == 0) s = "None";
			char* buf = new char[s.size() + 1];
			strcpy_s(buf, s.length() + 1, s.c_str());
			send_to_host(con->shared_from_this(), boost::asio::buffer(buf, s.size() + 1));
		}
		catch (std::exception&) {

			

		}

	}
	
	if ((idx = res.options.find("mkdir")) != end) {
	
		mkdir(*grp, con->shared_from_this(), res);
	
	}


	/*if (idx == 0) {		   //Command format: upload file_name

		char msg[] = { "Unknown file name" };
		send_to_host(con->shared_from_this(), buffer(msg, sizeof(msg)));

	}*/

}