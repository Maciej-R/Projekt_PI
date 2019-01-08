#include "LS.h"

void ls(User& usr, boost::shared_ptr<Connection> con, Result& res) {

	if (res.options.size() == 0) {
	
		//Iterate through directory, cast directory_entry to path for printig out only filenames in dir
		char p = 0;
		for (auto& it : fs::directory_iterator(usr.home_path())) {

			if (fs::is_directory(it)) continue;
			string name = static_cast<fs::path>(it).filename().string() + "\t";
			if (!(++p % 3)) name += "\n";
			char* buf = new char[name.size() + 1];
			strcpy_s(buf, name.size() + 1, name.c_str());
			send_to_host(con->shared_from_this(), boost::asio::buffer(buf, name.size() + 1));
				
		}
		return;
	
	}
	
	auto opt_it = res.options.find("path");
	fs::path where = usr.home_path();

	//Print filename + size
	opt_it = res.options.find("size");
	if (opt_it != res.options.end()) {

		for (auto& it : fs::directory_iterator(where)) {

			if (fs::is_directory(it)) continue;
			string text = static_cast<fs::path>(it).filename().string() + "\t";
			text += std::to_string(fs::file_size(it));
			char* buf = new char[text.size() + 1];
			strcpy_s(buf, text.size() + 1, text.c_str());
			send_to_host(con->shared_from_this(), boost::asio::buffer(buf, text.size() + 1));

		}
		return;

	}

	string text;
	opt_it = res.options.find("sort");
	if (opt_it != res.options.end()) {
	
		string param = opt_it->second;
		if (param == "size") {
		
			std::multimap<std::uintmax_t, string> files;
			for (auto& it : fs::directory_iterator(where)) {
			
				if (fs::is_directory(it)) continue;
				files.emplace(fs::file_size(it), it.path().filename().string());
			
			}

			for (auto it : files) {

				text += it.second + "\t";
				text += std::to_string(it.first);
				text += '\n';

			}
					
		}
		else if (param == "size_dec") {

			std::multimap<std::uintmax_t, string> files;
			for (auto& it : fs::directory_iterator(where)) {

				if (fs::is_directory(it)) continue;
				files.emplace(fs::file_size(it), it.path().filename().string());

			}

			for (auto it = files.rbegin(); it != files.rend(); ++it) {

				text = it->second + "\t";
				text += std::to_string(it->first);
				text += '\n';
				
			}
						
		}
		else if (param == "date") {

			std::multimap<fs::file_time_type, string> files;
			for (auto& it : fs::directory_iterator(where)) {

				if (fs::is_directory(it)) continue;
				files.emplace(fs::last_write_time(it), it.path().filename().string());

			}

			for (auto it : files) {

				//Filename
				text += it.second + "\t";
				auto ftime = it.first;
				//Time prepartion
				std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
				char* time_buf = new char[30];
				struct tm dfq;
				localtime_s(&dfq, &cftime);
				asctime_s(time_buf, 30, &dfq);
				//Time
				text += time_buf;
				text += '\n';
				//Send
				

			}

		}

	}
	else if (res.options.find("directories") != res.options.end()) {
	
		for (auto& it : fs::directory_iterator(where)) {

			if (!fs::is_directory(it)) continue;
			text += static_cast<fs::path>(it).filename().string() + "\t";
			text += '\n';

		}
	
	}
	else {
	
		for (auto& it : fs::directory_iterator(where)) {

			if (fs::is_directory(it)) continue;
			text += static_cast<fs::path>(it).filename().string() + "\t";
			text += '\n';

		}
	
	}

	opt_it = res.options.find("filter");
	if (opt_it != res.options.end()) {
	
		string& param = opt_it->second;
		if (param.find("!contains") != string::npos) {

			size_t start = param.find_first_of('\"');
			size_t end = param.find_last_of('\"');
			string sample{};
			std::copy(&param[start + 1], &param[end], std::back_inserter(sample));
			
			auto it = text.find_first_of(sample);
			if (it != string::npos) {
			
				string tmp{ std::move(text) };
				text.clear();
			
				size_t last_to_copy;
				size_t line_beg;
				for (line_beg = 0; it != string::npos || line_beg < tmp.size(); it = tmp.find_first_of(sample, it)) {

					//it indicates match while last_to_copy is being set 
					//on last character of previous line
					if (it != string::npos) {
					
						last_to_copy = it;
						while (last_to_copy > 0 && tmp[last_to_copy] != '\n') --last_to_copy;
					
					}
					else { 
						
						last_to_copy = tmp.size();
						it = tmp.size() - 2;

					}

					for (; line_beg != last_to_copy; ++line_beg) {

						text.push_back(tmp[line_beg]);

					}
					text += '\n';
					while (tmp[++it] != '\n'); //Skips rest of the linew with match
					line_beg = ++it;

				}
			
			}
		
		}
		else if (param.find("contains") != string::npos) {

			size_t start = param.find_first_of('\"');
			size_t end = param.find_last_of('\"');
			string sample{};
			std::copy(&param[start + 1], &param[end], std::back_inserter(sample));
			string tmp{std::move(text)};
			text.clear();
			auto it = tmp.find_first_of(sample);
			for (size_t line_beg; it != string::npos; it = tmp.find_first_of(sample, it)) {
			
				line_beg = it;
				//Begining of line
				while (tmp[line_beg] != '\n' && line_beg > 0) --line_beg;
				if(line_beg != 0) ++line_beg;
				//Copy line
				//Ends with \n => no need for range control
				for (; tmp[line_beg] != '\n'; ++line_beg) {
				
					text.push_back(tmp[line_beg]);
				
				}
				text += '\n';
				//Next seatch form next line
				if (it < tmp.size() && tmp[it] != '\n') ++it;
			
			}
		}

	}

	if (text.length() == 0) text += "None";

	char* buf = new char[text.size() + 1];
	strcpy_s(buf, text.size() + 1, text.c_str());
	send_to_host(con->shared_from_this(), boost::asio::buffer(buf, text.size() + 1));

}