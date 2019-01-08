#include "Mkdir.h"

void mkdir(User& usr, boost::shared_ptr<Connection> con, Result& res) {
	
	auto end = res.options.end();
	auto it = res.options.find("name");
	string& name = it->second;
	
	if (it == end) return;
	
	it = res.options.find("path");
	
	//User mustn't write out of his directory
	fs::path pth(usr.home_path());
	if (it != end) {
	
		pth.append(it->second);
	
	}

	pth.append(name);

	if (!fs::exists(pth)) {
	
		fs::create_directory(pth);
	
	}
	else {
	
		char buf[] = {"Path unavailable"};
		send_to_host(con->shared_from_this(), boost::asio::buffer(buf));
	
	}
	
}

void mkdir(Group& grp, boost::shared_ptr<Connection> con, Result& res) {

	if (res.options.size() < 1) return;

	auto it = res.options.find("path");
	auto end = res.options.end();
	//User mustn't write out of his directory
	fs::path pth(grp.home());
	if (it != end) {

		pth.append(it->second);

	}

	if (!fs::exists(pth)) {

		fs::create_directory(pth);

	}
	else {

		char buf[] = { "Path unavailable" };
		send_to_host(con->shared_from_this(), boost::asio::buffer(buf));

	}

}