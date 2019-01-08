#include "Upload.h"

namespace fs = std::experimental::filesystem;
using namespace boost::asio;

bool upload_handle(User& usr, boost::shared_ptr<Connection> con, Result& res) {

	if (res.options.size() != 2) throw bad_format{ "Not enough arguments" };

	long size{ atol( res.options.at("size").c_str() ) };

	/*if (idx == options.begin()) {		   //Command format: upload file_name

		char msg[] = { "Unknown file name" };
		send_to_host(con->shared_from_this(), buffer(msg, sizeof(msg)));

	}*/

	std::string name((std::find_if(res.options.begin(), res.options.end(), 
		[](auto& s)->bool {return s.first != "size"; }))
								->first);									   //Get filename
	fs::path path = usr.home_path();								   //Returns const
	path.append(name);												   //Path to file in user directory
	
	std::ofstream of("upload.log", std::ios::app);					//Log upload
	std::string info;
	info += usr.name();
	info += " upload of: ";
	info += name;
	of << info;
	of.close();

	of.open(path, std::ofstream::binary | std::ofstream::trunc);
	if (of.fail()) {

		to_log("Unable to open file " + name);						//Upload to user home directory
		throw std::runtime_error("Fail opening file");

	}

	const int maximum = 10000;				//Maximal download per one iteration 

	if (size < maximum) {

		//Only for small files
		char* buf = new char[size];											//Vector here causes bugs
		//mutable_buffer boost_buf(&buf, size);								//Input buffer
		ip::tcp::socket& soc_to_read = con->socket();						//For further data receive

		int sz = soc_to_read.read_some(boost::asio::buffer(buf, size));							//Read file
		of.write(buf, size);

		return true;

	}

	
	
	char* buf = new char[maximum];
	//mutable_buffer boost_buf(&buf, maximum);								//Input buffer
	ip::tcp::socket& soc_to_read = con->socket();						//For further data receive
	int sz;

	do {
			
		int to_read = size > maximum ? maximum : size;
		sz = soc_to_read.read_some(boost::asio::buffer(buf, size));							//Read file
		of.write(buf, sz);

	} while (size -= maximum > 0);

	return true;
			
}