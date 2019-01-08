#include "Delete.h"

void delete_command(User& usr, boost::shared_ptr<Connection> con, Result& res) {

	if (res.options.size() < 1) {
	
		char msg[] = { "Filename missing" };
		send_to_host(con->shared_from_this(), buffer(msg, sizeof(msg)));
		return;

	}

	fs::path path = usr.home_path();
	path.append(res.options.find("path")->second);

	if (fs::is_directory(path)) {
	
		if (fs::remove_all(path)) return;
		char msg[] = { "Deletion failed" };
		send_to_host(con->shared_from_this(), buffer(msg, sizeof(msg)));
		return;
	
	}

	if (fs::exists(path)) {
	
		if (fs::remove(path)) return;
		char msg[] = { "Deletion failed" };
		send_to_host(con->shared_from_this(), buffer(msg, sizeof(msg)));
		return;
	
	}

}