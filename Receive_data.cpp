#include "Receive_Data.h"

std::mutex cout_mutex;
std::timed_mutex socket_mutex;
std::atomic_bool receive_printed;

void receive_data(boost::shared_ptr<Connection> con) {

	char buf[1024];
	boost::asio::mutable_buffer boost_buf(buf, 1024);
	ip::tcp::socket& input_soc = con->socket();

	io_context& cont = input_soc.get_io_context();

	try {

		while (!cont.stopped()) {

			int rec;// = input_soc.receive(boost_buf);;

			{
				std::unique_lock<std::timed_mutex> lock(socket_mutex, std::defer_lock);
				while (!lock.try_lock_for(std::chrono::milliseconds(500))) {

					std::this_thread::yield();
					
				}

				//boost::system::error_code error;
				rec = input_soc.receive(boost_buf);
				//if(error == boost::asio::error::eof);
					
			}

			if (rec) {
				
				std::lock_guard<std::mutex> lck(cout_mutex);	
				//Locked in case of client message out print
				cout << "\n";
				cout << "!!!: ";
				cout.write(buf, rec);
				cout << "\n";
				cout << Config::prompt();
				receive_printed.store(true, std::memory_order_relaxed);

			}//Unlocked - out of scope
			else std::this_thread::sleep_for(std::chrono::milliseconds(500));

		}

	}
	catch (boost::system::system_error& e) {
	
		cout << "Connection terminated";
			
	}

}

Receiver::Receiver(boost::shared_ptr<Connection> con) :
	class_con(con->shared_from_this()), thread_receive(
		receive_data, con->shared_from_this()){

	//std::thread t2(std::thread( [&]() { receive_data(con->shared_from_this(), end_operations); }));
	//thread_receive.swap(t2);
}
Receiver::~Receiver() {

	char buf[]{ "wake" };
	class_con->socket().shutdown(ip::tcp::socket::shutdown_both);
	/*boost::asio::io_context io_context;

	Connection::connection_pointer con = Connection::create(io_context);					//Socket in use
	ip::tcp::socket& socket = con->socket();
	ip::tcp::endpoint endp = class_con->socket().local_endpoint;  //Target
	
	socket.connect(endp);*/
	//class_con->socket().send(boost::asio::buffer(buf, 5));
	thread_receive.join();

}