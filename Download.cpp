#include "Download.h"

void download_file(boost::shared_ptr<Connection> con, std::string& name) {

	int buf_max = 10000;
	vector<char> buf(buf_max);
	//Saves at path from config + filename
	std::ofstream of(Config::downloads_path().append(name), std::ofstream::binary | std::ofstream::trunc);
	ip::tcp::socket& soc = con->socket();

	try {

		do {

			int size = soc.read_some(boost::asio::buffer(buf, buf_max));
			of.write(buf.data(), size);

		//Server closes socket at end of transmission
		} while (soc.is_open());

	}
	catch (std::exception& e) {}


}