#include "Connection.h"

Connection::connection_pointer Connection::create(boost::asio::io_context& cont) {

	return connection_pointer(new Connection(cont));

}


ip::tcp::socket& Connection::socket() { return socket_; }