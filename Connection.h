#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;

class Connection : public boost::enable_shared_from_this<Connection> {

private:

	ip::tcp::socket socket_;
	Connection(boost::asio::io_context& cont) : socket_(cont) {}

public:

	typedef boost::shared_ptr<Connection> connection_pointer;								//Exists as long as in use

	static connection_pointer create(boost::asio::io_context&);

	ip::tcp::socket& socket();

};