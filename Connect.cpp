#include "Connect.h"

extern std::mutex cout_mutex;
extern std::timed_mutex socket_mutex;
extern std::atomic_bool receive_printed;

void upload_command(string&, boost::shared_ptr<Connection>);
bool check();
bool check_for_directory(fs::path&);
void configure_from_console();

class Separation {

public:

	bool operator()(char c) { return isspace(c); }

};

bool connect() {
	
	if(!check()) return true;

	boost::asio::io_context io_context;
	ip::tcp::resolver resolver(io_context);				

	//Socket in use
	Connection::connection_pointer con = Connection::create(io_context);					
	ip::tcp::socket& socket = con->socket();
	ip::tcp::endpoint endp;

	{

		string input_address;
		bool first = true;

		cout << "Inscribe destination IP address \n"
			<< "Or press enter to connect to local host \n IP: ";

		do {

			if (!first) cout << "Check address \n IP: ";
			else first = false;
			std::getline(cin, input_address);
			//Default - local host (enter pressed)
			if (input_address == "") {

				input_address = "127.0.0.1";
				break;

			}
		
			//Ipv4 regex won't catch invalid address with wrong digit at second position, example - 292
		} while (!std::regex_match(input_address, std::regex("(([0-2])?([0-9]){1,2}\.){3}(([0-2])?[0-9]{1,2}){1}"))	//IPv4 
			/*Ipv6*/ && !std::regex_match(input_address, std::regex("(::)?(([a-f0-9]){1,4}:(:)?){0,7}(([a-f0-9]){1,4}){1}")));

		//Destination
		endp.address(ip::address::from_string(input_address));
		endp.port(50000);

	}
		
	//Try to connect and check result
	boost::system::error_code error;
	socket.connect(endp, error);
	if (error) {
	
		cout << "Unable to connect \n \n";
		return false;
		
	}
	//Receiving data from server
	Receiver rcv(con->shared_from_this());
		
	{
		//Connection settings
		socket_base::keep_alive option_keep(true);
		ip::tcp::no_delay option_delay(true);
		socket.set_option(option_keep);
		socket.set_option(option_delay);
	}

	std::string command;
	std::string input;
	bool printed = true;

	for (;command != "quit";){

		try {

			//Relaxed order - negligible importance (propmt outprint)
			if (!printed && !receive_printed.load(std::memory_order_relaxed)) {

				std::lock_guard<std::mutex> lck(cout_mutex);
				cout << Config::prompt();
				printed = true;
			
			}

			//Clear previous value
			command.clear();		

			std::getline(cin, input);			//Try to read from stdin
			if (cin.fail()) {

				cin.clear();														//Prepare for next iteration
				std::this_thread::sleep_for(std::chrono::milliseconds(500));		//Sleep if there's no command			
				continue;															//Rest is superflous (parsing command)

			}

			printed = false;		//Command input occured
			receive_printed.store(false, std::memory_order_relaxed);

			//Separate command from options
			auto x = std::find_if(input.begin(), input.end(), [](char c) { return isspace(c); });
			std::copy(input.begin(), x, std::back_inserter(command));

			int length = input.length() - command.length();

			if (command == "upload") {

				Result res;

				try {

					res = std::move(Result(const_cast<char*>(input.c_str()), input.length(), ' '));

				}
				catch (bad_format& e) {

					cout << "Check command syntax \n";

				}

				/*std::vector<std::string> options(std::move(

					separate_options(const_cast<char*>(input.c_str()) + command.length(), length, Separation{})));*/
				//Pointer to input + command length + 1 for space
				//Skips command (not first space - for predicate usage); index to last + 1 (iteration <beg;end) )

				if (!res.options.size()) throw bad_format{ "File name required" };

				input.insert(
					std::find_if(input.begin() += command.length(), input.end(), [](char c)->bool { return isalpha(c); }),
					'-'); //To mark file name begining

				char path_beg = (res.options.begin()->first).at(0);
				bool win_path = std::regex_search(res.options.begin()->first, std::regex("^([A-Z]):"));		//Checks for Windows style 
				fs::path path = path_beg == '/' || win_path ? res.options.begin()->first : Config::locals_path().append(res.options.begin()->first);
				
				if (fs::is_directory(path)) {

					cout << "Directory unallowed!" << "\n";
					continue;

				}
				if (!fs::exists(path)) {

					cout << "Unexisting file" << "\n";
					continue;

				}

				input += " -size ";
				std::stringstream ss_size;

				try {
					ss_size << fs::file_size(path);
				}
				catch (std::exception& e) {

					cout << e.what();

				}

				input += ss_size.str();

				upload_command(input, con->shared_from_this());
				upload_file(path, con->shared_from_this());


			}
			else if (command == "download") {
			
				/*std::unique_lock<std::timed_mutex> lock(socket_mutex, std::defer_lock);
				while (!lock.try_lock_for(std::chrono::milliseconds(500))) {

					std::this_thread::yield();

				}*/

				Result res;

				try {

					res = std::move(Result(const_cast<char*>(input.c_str()), input.length()));

				}
				catch (bad_format& e) {

					cout << "Check command syntax \n";
				
				}

				if (res.options.size() < 1) {
				
					cout << "More arguments needed \n";
					continue;
				
				}
				auto it = res.options.find("path");
				string& name = it->second;
				fs::path pth(name);
				if (res.options.find("compress") != res.options.end()) {
				
					pth.replace_extension(".gz");
				
				}
				it->second = pth.string();

				try {

					boost::asio::io_context io_context_;
					ip::tcp::acceptor acceptor(io_context_, ip::tcp::endpoint(ip::tcp::v4(), 50001));

					Connection::connection_pointer channel =									//Create a connection
						Connection::create(acceptor.get_executor().context());

					upload_command(input, con->shared_from_this());
					acceptor.accept(channel->socket());
					
					download_file(channel->shared_from_this(), name);
					

				}
				catch (std::exception& e) {
				
					cout << e.what();
				
				}
					
			}
			else if (command == "help") {
			
				std::ifstream stream("help.txt");
			
				std::lock_guard<std::mutex> lck(cout_mutex);
				cout << stream.rdbuf();

			}
			else if (command == "create") {
			
				std::vector<std::string> options(std::move(
					separate_options(const_cast<char*>(input.c_str()) + command.length(), length, Separation{})));

				std::ofstream of(Config::locals_path().append(options[0]));

				cout << "File content: \n";

				cin.get(*(of.rdbuf()));

				printed = false;

			}
			else if (command == "config") {

				configure_from_console();

			}
			else {
			
				//Server is the recipient of command
				upload_command(input, con->shared_from_this());
			
			}

			

		}
		catch (std::exception& e) {
		
			cout << e.what() << "\n";
			throw;
		
		}

	}

	if (command == "quit") {

		io_context.stop();
		return true;

	}

	return false;


}

inline void upload_command(string& input, boost::shared_ptr<Connection> con) {

	std::vector<char> buf(input.size());
	std::copy(input.begin(), input.end(), buf.begin());
	boost::system::error_code error;

	try {

		//std::unique_lock<std::timed_mutex> lck(socket_mutex, std::defer_lock);
		//if (!lck.try_lock_for(std::chrono::milliseconds(10000))) throw std::runtime_error("Unable to send \n");
		write(con->socket(), buffer(buf));
	}
	catch (std::runtime_error& e) {

		cout << e.what();

	}

}

bool check() {

	bool b1 = check_for_directory(fs::current_path().append("Downloads"));
	bool b2 = check_for_directory(fs::current_path().append("Locals"));

	return b1 && b2;

}

bool check_for_directory(fs::path& pth) {

	if (!fs::exists(pth)) {

		fs::create_directory(pth);

	}
	else if (!fs::is_directory(pth)) {

		cout << "Directory " << pth.filename() << " needed at current path \n";
		return false;

	}

	return true;

}

void configure_from_console() {

	cout << "Entering configuration mode \nEnter \"end\" to call a halt \n";
	string input;
	while (input.find("end") == string::npos) {
	
		getline(cin, input);
		Result res(const_cast<char*>(input.c_str()), input.length(), ' ');
		if (res.command == "help") {
		
			cout << "Available options are listed in client.cfg configuration file \n"
				<< "Proper syntax is: option_name value \n"
				<< "Example: downloads D:\\MyDownloads \n";
		
		}
		else if (res.command == "downloads") {
		
			auto x = analyze_path(res.options.begin()->first);
			Config::downloads_path_change(x);
		
		}
		else if (res.command == "locals") {

			auto x = analyze_path(res.options.begin()->first);
			Config::locals_path_change(x);

		}
		else if (res.command == "prompt") {

			string opt = res.options.begin()->first;
			Config::prompt_change(opt);

		}
	
	}

}

/*else {


				std::vector<std::string> options(std::move(
					separate_options(const_cast<char*>(input.c_str()) + command.length() + 1, length)));
				//Pointer to input + command length + 1 for space

				for (auto x : options) cout << x << " ";


			}*/

/*


	if (error == boost::asio::error::eof) {

		cout << "Connection terminated by server";
		// Connection closed cleanly by peer.

	}
	else if (error) {
		throw; // Some other error.
	}


*/

/*while (cin.rdbuf()->in_avail() == 0) {

				if (receive_printed.load(std::memory_order_relaxed)) {

					receive_printed = false;
					std::lock_guard<std::mutex> lck(cout_mutex);
					cout << "$ ";

				}
				std::this_thread::sleep_for(std::chrono::milliseconds(500));


			}*/