#pragma once
#include <map>
#include <string>
#include <vector>
#include "SeparateOptions.h"
#include "OptionValue.h"
#include "OptionIndex.h"

using std::map;
using std::string;
using std::vector;

class Result{
public:

	map<string, string> options;
	string command;

	Result(char*, int);
	Result() {}
	
};

int preproces_command(char*, int);