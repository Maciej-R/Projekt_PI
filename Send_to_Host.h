#pragma once
#include <boost/asio.hpp>
#include <future>
#include "Server.h"

using namespace boost::asio;
class Connection;

//inline void send_to_host(ip::tcp::socket&, boost::asio::mutable_buffer&);
void send_to_host(ip::tcp::socket&, boost::asio::mutable_buffer);
void send_to_host(boost::shared_ptr<Connection>, boost::asio::mutable_buffer);