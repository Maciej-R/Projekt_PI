#pragma once
#include <map>
#include <string>
#include <vector>
#include "Separate_Options.h"
#include "OptionValue.h"

using std::map;
using std::string;
using std::vector;

class Result{
public:

	map<string, string> options;
	string command;

	Result(char*, int);
	Result(char*, int, char);
	Result() {}
	
};

int preproces_command(char*, int);