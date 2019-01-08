#include "Download.h"

void download_handle(User& usr, boost::shared_ptr<Connection> con, Result& res) {

	fs::path path;
	auto it = res.options.find("path");
	if (it != res.options.end()) {

		path = usr.home_path();
		path.append(it->second);

	}
	else {
	
		path = usr.home_path();
		//No other options possible
		path.append(res.options.begin()->first);
	
	}
		
	if (!fs::exists(path)) {
	
		char buf[] = {"No file with given path"};
		send_to_host(con->shared_from_this(), boost::asio::buffer(buf));
		return;

	}
	
	if ((it = res.options.find("compress")) != res.options.end()) {
	
		path = compress(path);

	}
	unsigned long long size = fs::file_size(path);

	int buf_max = 10000;
	vector<char> buf(buf_max);
	std::ifstream in(path, std::ios_base::in | std::ios_base::binary);

	//Connect to host on 50001 port
	io_context io_context_;
	Connection::connection_pointer con_to_host = Connection::create(io_context_);

	try {

		ip::tcp::endpoint endp((con->socket()).remote_endpoint().address(), 50001);

		con_to_host->socket().connect(endp);
		
	}
	catch (std::exception& e) {

		to_log(e.what());

	}

	if (size < buf_max) {

		in.read(buf.data(), size);
		write(con_to_host->socket(), boost::asio::buffer(buf, size));
		return;

	}
	do {

		int to_send = size > buf_max ? buf_max : size;
		in.read(buf.data(), to_send);
		write(con_to_host->socket(), boost::asio::buffer(buf, to_send));
		size -= to_send;

	} while (size > 0);

	con_to_host->socket().close();

}