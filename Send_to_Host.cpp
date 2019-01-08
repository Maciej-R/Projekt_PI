#include "Send_to_Host.h"

void send_to_host(ip::tcp::socket& soc, mutable_buffer buf) {

	//Immediately starts send operation on given socket
	std::async(std::launch::async, [&soc, buf]() { soc.send(buf); });

}

void send_to_host(boost::shared_ptr<Connection> con, boost::asio::mutable_buffer buf) {

	ip::tcp::socket& soc = con->socket();
	std::async(std::launch::async, [&soc, buf]() { soc.send(buf); });

}


/*void send_to_host(ip::tcp::socket& soc, mutable_buffer& buf) {

	std::thread sender{ [&]() { soc.send(buf); } };
	sender.join();

}*/