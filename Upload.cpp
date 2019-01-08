#include "Upload.h"

void upload_file(fs::path& path, boost::shared_ptr<Connection> con) {

	unsigned long long size = fs::file_size(path);

	int buf_max = 10000;
	vector<char> buf(buf_max);
	std::ifstream in(path);

	//All at once
	if (size < buf_max) {
	
		in.read(buf.data(), size);
		write(con->socket(), boost::asio::buffer(buf, size));
		return;
	
	}
	//File bigger than buffer
	do {
		
		int to_send = size > buf_max ? buf_max : size;
		in.read(buf.data(), to_send);
		write(con->socket(), boost::asio::buffer(buf, to_send));

	} while (size -= buf_max > 0);
		
}