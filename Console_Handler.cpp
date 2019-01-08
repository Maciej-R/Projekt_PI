#include "Console_Handler.h"

using std::cin;
using std::cout;
namespace fs = std::experimental::filesystem;
extern std::string server_main_directory;
extern std::vector<User> Users_Global;

void handle_console(){

	std::string input;
	fs::path working_path = fs::current_path().append(server_main_directory);
	Result res;
	bool printed = false;

	do{

		if (!printed) {

			cout << '$';
			printed = true;

		}

		std::getline(cin, input);			//Try to read from stdin
		if (cin.fail()) {		
		
			cin.clear();														//Prepare for next iteration
			std::this_thread::sleep_for(std::chrono::milliseconds(500));		//Sleep if there's no command			
			continue;															//Rest is superflous (parsing command)
		
		}
		
		//ELSE
		//Separate command from options
		res = std::move(Result(const_cast<char*>(input.c_str()), input.length()));
		string& command(res.command);
		printed = false;
		
	
		if (command == "pwd") {
			
			std::cout << working_path << "\n";

		}
		else if (command == "ls") {
		
			//Iterate through directory, cast directory_entry to path for printig out only filenames in dir
			char p = 0;
			for (auto& it : fs::directory_iterator(working_path)) {
			
				cout << static_cast<fs::path>(it).filename() << "\t";
				if (!(++p % 3)) cout << "\n";

			}
			cout << "\n";
		
		}
		else if (command == "space") {
		
			//Disk space info
			fs::space_info info = fs::space(working_path);
			cout << "Free: " << info.free << "   " << "Avaiable: " << info.available << "\n";
		
		}
		else if (command == "main") {
		
			cout << server_main_directory << "\n";
		
		}
		else if (command == "users") {
		
			char p = 0;
			for (auto x : Users_Global) {

				cout << x.name() << "\t";
				if (!(++p % 5)) cout << "\n";

			}
			cout << "\n";
		
		}
		else if (command == "groups") {

			char p = 0;
			for (auto x : Groups_Global) { 
				
				cout << x.name() << "\t";
				if (!(++p % 5)) cout << "\n";

			}
			cout << "\n";

		}
		else if (command == "erase_groups_data") {
		
			cout << "Confirm erasure? [y/n]:  ";
			char c;
			cin >> c;
			if (c == 'n') continue;
			for (auto& it : fs::directory_iterator(working_path.append("groups"))) {

				fs::remove(it);

			}
			std::ofstream of("Groups_Global.data", std::ios_base::trunc);
			of << 0;
			of.close();

			Groups_Global.clear();
		
		}
		else if (command == "active") {
		
			cout << Active::active() << "\n";
		
		}
		else if (command == "help") {

			std::ifstream stream("help.txt");

			cout << stream.rdbuf();

		}
		else if (command == "setmd") {

			server_main_directory = res.options.begin()->first;

		}
		else if (command == "decompress") {
		
			decompress(res.options.find("path")->second);

		}
	
	} while (res.command != "quit");

}