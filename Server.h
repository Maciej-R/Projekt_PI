#pragma once
#include "Start_Handling_User.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

class Connection : public boost::enable_shared_from_this<Connection> {						//Manages connection creation

private:

	tcp::socket socket_;
	Connection(boost::asio::io_context& cont) : socket_(cont) {}

public:

	typedef boost::shared_ptr<Connection> connection_pointer;								//Exists as long as in use

	static connection_pointer create(boost::asio::io_context&);

	tcp::socket& socket() { return socket_; }

	void start();										//Start processing data form connection

};



class Server {

private:

	tcp::acceptor server_acceptor;
	void start_listening();																	//Asynchronoulsy awaits connection request and calls handle
	void handle_accept(Connection::connection_pointer, const boost::system::error_code&);	//Handles connection request

public:

	Server(boost::asio::io_context&);

};