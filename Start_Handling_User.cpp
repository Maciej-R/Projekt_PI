#include "Start_Handling_User.h"

using namespace boost::asio;

extern std::vector<User> Users_Global;

void start_handling_user(boost::shared_ptr<Connection> con) {

	char buf[1024];
	mutable_buffer boost_buf(buf, 1024);
	ip::tcp::socket& soc_to_read = con->socket();

	//Connection is kept alive and data is tranfered with no delay
	socket_base::keep_alive option_keep(true);
	ip::tcp::no_delay option_delay(true);
	soc_to_read.set_option(option_keep);
	soc_to_read.set_option(option_delay);

	{
		char welcome[] = { "Welcome to server \n" };
		char log[] = { "Log in or create user: " };
		try {
			send_to_host(soc_to_read, { welcome, sizeof(welcome) });
			send_to_host(soc_to_read, { log, sizeof(log) });
		}
		catch (std::exception& e) {

			to_log("Send Error");

		}
	}

	io_context& cont = soc_to_read.get_io_context();

	try {

		while (!cont.stopped()) {

			int sz = soc_to_read.receive(boost_buf);

			std::string s;
			try {
				std::copy(buf, std::find_if(buf, buf + sz, [](char c) { return isspace(c); }), std::back_inserter(s));	//Get command
			}
			catch (std::exception& e) {
			
				e.what();

			}

			User* curr_usr = nullptr;

			if (s == "login") {

				std::vector<std::string> credentials = separate_options(&buf[s.size()], sz - s.size(), [](char c)->bool { return std::isspace(c); });					//Command format : login user_name password
				if (credentials.size() < 2) {
				
					char msg[] = "Username and password needed";
					send_to_host(soc_to_read, { msg, sizeof(msg) });
					continue;
				
				}

				std::vector<User>::iterator x;
				x = find_user(credentials[0]);
					//x = std::find_if(Users_Global.begin(), Users_Global.end(),
						//[&](User& usr) {return option_value(credentials[0]) == usr.name(); });
				
				if (x == Users_Global.end()) {
					
					char msg[] = "Wrong username";
					send_to_host(soc_to_read, { msg, sizeof(msg) });
					continue;
					
				}
				if (x->password() == credentials[1]) curr_usr = &*x;
				
				if (curr_usr == nullptr) {
				
					char msg[] = "Wrong password";
					send_to_host(soc_to_read, { msg, sizeof(msg) });
					continue;

				}
				else {

					std::async(handle_user, std::ref(*curr_usr), con->shared_from_this());
					return;

				}

				//continue;

			}
			else if (s == "adduser") {

				try {

					if (s.size() >= sz) throw bad_format{ "Not enough arguments" };

					string name;
					if ( (name = adduser(&buf[s.size() + 1], sz - s.size() - 1)) == string{}) {

						char message[] = { "User already exists" };
						con->socket().send(buffer(message));

					}
					else {//Success, proceed
					
						char message[] = { "User created" };
						con->socket().send(buffer(message));
						auto pj = find_user(name)->name();
						curr_usr = &*find_user(name);
						handle_user(*curr_usr, con);

					}

				}
				catch (bad_format& bf) {

					const char* message = bf.what();
					int i = 0;
					for (; message[i] != '\0'; ++i) {}
					con->socket().send(const_buffer(message, i));

				}
				

			}
			else {

				char message[] = { "Credentials required" };
				con->socket().send(buffer(message));

			}

		}
	
	}
	catch (std::exception& e) {
	
		string s = e.what();
	
	}
	//if (buf[0] = 'q') std::cout << 111111;

}