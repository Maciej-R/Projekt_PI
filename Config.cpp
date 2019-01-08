#include "Config.h"

fs::path Config::downloads_path_;
fs::path Config::locals_path_;
std::string Config::prompt_;

Config::Config(){

	if (fs::exists(fs::current_path().append("client.cfg"))) read_config();
	else default_config();

}

void Config::default_config() {

	downloads_path_ = fs::current_path().append("Downloads");
	locals_path_ = fs::current_path().append("Locals");
	prompt_ = "$ ";

}

void Config::read_config() {

	std::ifstream in(fs::current_path().append("client.cfg"));
	//Read whole file
	std::string input((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
	input = std::move(delete_comment(input));

	Result res;

	try {

		res = std::move(Result(const_cast<char*>(input.c_str()), input.length()));

	}
	catch (bad_format&) {

		default_config();

	}

	auto end = res.options.end();
	auto it = res.options.find("downloads");
	if (it != end) {
	
		downloads_path_ = analyze_path(it->second);
	
	}
	else {
	
		downloads_path_ = fs::current_path().append("Downloads");
		if (!fs::exists(downloads_path_)) fs::create_directory(downloads_path_);

	}
	if ((it = res.options.find("locals")) != end) {

		locals_path_ = analyze_path(it->second);

	}
	else {

		locals_path_ = fs::current_path().append("Locals");
		if (!fs::exists(locals_path_)) fs::create_directory(locals_path_);

	}
	if ((it = res.options.find("prompt")) != end) {

		prompt_ = it->second;
		prompt_ += " ";

	}
	else {

		prompt_ = "$ ";

	}

}

std::string Config::delete_comment(const std::string& obj) {

	string ret;
	string sample{"//"};
	
	auto it = obj.find_first_of(sample);
	if (it != string::npos) {
				
		size_t last_to_copy;
		size_t line_beg;
		for (line_beg = 0; it != string::npos || line_beg < obj.size(); it = obj.find_first_of(sample, it)) {

			//it indicates match while last_to_copy is being set 
			//on last character of previous line
			if (it != string::npos) {

				last_to_copy = it;
				
			}
			else {

				last_to_copy = obj.size();
				//Sets it the way in won't cause error at for instructions calculations
				it = obj.size() - 2;

			}

			for (; line_beg != last_to_copy; ++line_beg) {

				ret.push_back(obj[line_beg]);

			}
			//if(last_to_copy)ret += '\n';
			while (obj[++it] != '\n'); //Skips rest of the line with match
			line_beg = ++it;

		}

	}

	return ret;

}

fs::path analyze_path(const std::string& str) {

	char path_beg = str.at(0);
	bool win_path = std::regex_search(str, std::regex("^([A-Z]):"));		//Checks for Windows style 
	fs::path path = path_beg == '/' || win_path ? str : fs::current_path().append(str);
	return path;

}

void Config::make() {

	Config();

}

fs::path Config::locals_path() {

	return Config::locals_path_;

}

const std::string& Config::prompt() {

	return Config::prompt_;

}

fs::path Config::downloads_path() {

	return Config::downloads_path_;

}

void Config::prompt_change(const std::string& str) {

	Config::prompt_ = str;

}

void Config::downloads_path_change(const fs::path& pth) {

	Config::downloads_path_ = pth;

}

void Config::locals_path_change(const fs::path& pth) {

	Config::locals_path_ = pth;

}
