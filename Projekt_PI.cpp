//#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include "Program_Initialization.h"
#include "Console_Handler.h"

class Initialize;

vector<User> Users_Global;
vector<Group> Groups_Global;
std::string server_main_directory{};

int main() {

	boost::asio::io_context context;
	Initialize init(context);

	std::thread processing_loop(
		[&]() { context.run(); }
	);

	std::thread console(handle_console);

	//Program runs as long as there's no quit order from console
	
	console.join();									
	context.stop();
	processing_loop.join();

}
