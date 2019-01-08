#pragma once
#include "Server.h"
#include "Group.h"
#include "membership.h"

//Class to assure proper data save upon destructor call at program exit

class Initialize {					

private:

	void read_data();
	void save_data();
	bool check_requirements();
	void post_init();
	//void start_server();
	Server working_server;

public:

	Initialize(boost::asio::io_context& context) : working_server(context) {
		
		if(check_requirements())			//Data files existance + preparation of required directories
			read_data();
		//start_server();  
	}
	~Initialize() { save_data(); }

};


