#include "Handle_User.h"

using namespace boost::asio;
using namespace std::chrono;

extern std::vector<User> Users_Global;
class Active;

void handle_user(User& usr, boost::shared_ptr<Connection> con){

	Active::increase();

	try {
	
		char message[] = { "Success!" };
		con->socket().send(buffer(message));

	}
	catch (std::exception& e) {
	
		to_log(e.what());
	
	}

	try {

		char buf[1024];
		mutable_buffer boost_buf(buf, 1024);
		ip::tcp::socket& soc_to_read = con->socket();

		io_context& cont = soc_to_read.get_io_context();

		while (!cont.stopped()) {																		//Processing loop

			int sz = soc_to_read.receive(boost_buf);

			Result res;
			try {

				res = std::move(Result(buf, sz));

			}
			catch (bad_format& e) {

				char buf[] = { "Check command syntax" };
				send_to_host(con->shared_from_this(), boost::asio::buffer(buf));

			}

			std::string& s = res.command;
			
			if (s == "quit") break;
			else if (s == "addgroup") {

				try {

					if (!addgroup(res, usr)) {

						char message[] = { "Group already exists" };
						con->socket().send(buffer(message));

					}
					else {
					
						char message[] = { "Group created!" };
						con->socket().send(buffer(message));

					}

				}
				catch (bad_format& bf) {

					const char* message = bf.what();
					int i = strlen(message);
					con->socket().send(const_buffer(message, i));

				}

			}
			else if (s == "upload") {

				upload_handle(usr, con->shared_from_this(), res);

			}
			else if (s == "group") {
			
				manage_group(usr, con->shared_from_this(), res);
			
			}
			else if (s == "ls") {
			
				ls(usr, con->shared_from_this(), res);

			}
			else if (s == "download") {
			
				download_handle(usr, con->shared_from_this(), res);
			
			}
			else if (s == "mkdir") {
			
				mkdir(usr, con->shared_from_this(), res);
			
			}
			else if (s == "delete") {
			
				delete_command(usr, con->shared_from_this(), res);
			
			}
		}

		Active::decrease();

	}
	catch (std::exception& e) {
	
		to_log(e.what());
		Active::decrease();
		/*std::tm cdate{};
		string curr_date;
		curr_date = (char)cdate.tm_mday + "." + (char)(1 + cdate.tm_mon); 
		curr_date += "." + (1900 + cdate.tm_year);
		std::ofstream of(curr_date += ".log", std::ios::app);
		of << e.what();
		of << '\n';*/

	}

}