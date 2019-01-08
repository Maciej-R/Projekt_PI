#include "Result.h"

Result::Result(char* input, int end) {

	if (end < 1) return;

	end = preproces_command(input, end);

	//First blank
	char* it = std::find_if(input, input + end, [](char c) { return isspace(c); });
	//Command
	std::copy(input, it, std::back_inserter(command));
	//Correction due to parameters offset
	char dif = it - input;
	input += dif;
	end -= dif;

	//Single options, form: option value
	vector<string> opt(std::move(separate_options(input, end)));
	string val;

	//To [option] = value
	for (auto x : opt) {
	
		val = option_value(x);
		string appelative;
		for (auto c : x) {
		
			if (isspace(c)) break;
			appelative.push_back(c);
		
		}
		options.emplace(appelative, val);
	
	}

}

int preproces_command(char* command, int end) {

	//Removes multiple spaces
	std::unique(command, command + end, [](char c, char d) { return isspace(c) && isspace(d); });
	int last_not_space = end - 1;
	while (isspace(command[last_not_space])) --last_not_space;
	//Back to <beg;end) range
	++last_not_space;
	return last_not_space;

}

Result::Result(char* input, int end, char delim) {

	if (end < 1) return;

	end = preproces_command(input, end);

	//First blank
	char* it = std::find_if(input, input + end, [](char c) { return isspace(c); });
	//Command
	std::copy(input, it, std::back_inserter(command));
	//Correction due to parameters offset
	char dif = it - input;
	input += dif;
	end -= dif;

	//Single options, form: option value
	vector<string> opt(std::move(separate_options(input, end, delim)));
	string val;

	//To [option] = value
	for (auto x : opt) {

		val = option_value(x);
		string appelative;
		for (auto c : x) {

			if (isspace(c)) break;
			appelative.push_back(c);

		}
		options.emplace(appelative, val);

	}

}