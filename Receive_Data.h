#pragma once
#include "Connection.h"
#include "Config.h"
#include <boost/asio.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>

using std::cout;

//Handles messages form server
void receive_data(boost::shared_ptr<Connection>);
class Receiver {

	boost::shared_ptr<Connection> class_con;
	std::thread thread_receive;

public:

	Receiver(boost::shared_ptr<Connection>);
	~Receiver();

};