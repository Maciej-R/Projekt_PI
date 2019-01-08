#include "Server.h"

using namespace boost::asio;

void Server::handle_accept(Connection::connection_pointer ptr, const boost::system::error_code& err) {

	if (!err)
	{
		ptr->start();
	}

	start_listening();														//Continue listening

}

void Server::start_listening() {

	Connection::connection_pointer channel =									//Create a connection
		Connection::create(server_acceptor.get_executor().context());
																		
	server_acceptor.async_accept(channel->socket(),							//Starts asynchronous listening on socekt from above
		boost::bind(&Server::handle_accept, this, channel,
			boost::asio::placeholders::error));

}

//Acceptor initializaion and begining of listening with given parameters
Server::Server(io_context& cont) : server_acceptor(cont, ip::tcp::endpoint(ip::tcp::endpoint(ip::tcp::v4(), 50000))) {		
	start_listening();
}

Connection::connection_pointer Connection::create(boost::asio::io_context& cont) {

	return connection_pointer(new Connection(cont));

}

void Connection::start() { start_handling_user(shared_from_this()); }