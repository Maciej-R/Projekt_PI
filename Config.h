#pragma once
#include <filesystem>
#include <string>
#include <fstream>
#include <regex>
#include <iterator>
#include "Result.h"

namespace fs = std::experimental::filesystem;

class Config{

	std::string delete_comment(const std::string&);
	static fs::path downloads_path_;
	static fs::path locals_path_;
	static std::string prompt_;
	Config();
	
public:

	static fs::path locals_path();
	static fs::path downloads_path();
	static const std::string& prompt();
	static void prompt_change(const std::string&);
	static void downloads_path_change(const fs::path&);
	static void locals_path_change(const fs::path&);
	void read_config();
	void default_config();

	static void make();
	
};

//Function checks whether argumetn is full path
//otherwise appends it to current path
fs::path analyze_path(const std::string&);