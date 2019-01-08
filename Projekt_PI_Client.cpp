//#include "pch.h"
#include "Connect.h"

int main()
{

	//Marks weather program should quit
	bool quit_flag = false;		
	//Loads configuration
	Config::make();

	while (!quit_flag) {		//Processing in loop in case of failure

		try
		{

			quit_flag = connect();

		}
		catch (boost::system::system_error& e)
		{

			cout << "Connection failure \n" << R"(Type "try" to attempt to reconnect or "quit" to close program)" << "\n";
			std::string input;

			while(1) {

				input.clear();						//Clear previous value

				std::getline(cin, input);			//Try to read from stdin
				if (cin.fail()) {

					cin.clear();														//Prepare for next iteration
					std::this_thread::sleep_for(std::chrono::milliseconds(500));		//Sleep if there's no command			
					continue;															//Rest is superflous (parsing command)

				}

				if (input == "try") {

					break;				//Control back to processing loop

				}

				else if (input == "quit") {
				
					quit_flag = true;
					break;
				
				}

			} 

		}

	}

	return 0;
}



/*void try_sep() {

	std::string s;

	do {

		std::getline(cin, s);
		if (cin.fail()) {

			cin.clear();														//Prepare for next iteration
			std::this_thread::sleep_for(std::chrono::milliseconds(500));		//Sleep if there's no command			
			continue;															//Rest is superflous (parsing command)

		}

		std::vector<std::string> options(separate_options(const_cast<char*>(s.c_str()), s.size()));

		for (auto x : options) cout << x;

	} while (s != "q");
}
*/