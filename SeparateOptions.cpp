#include "SeparateOptions.h"

using std::vector;
using std::string;

vector<string> separate_options(char* opt, int end, char delim) {

	int it = 0;
	vector<string> result;

	for (; it < end; ++it) {		//Iterates through whole input without command name

		if (opt[it] == delim) {		//Options begin with delimiter

			int l = ++it;
			for (; l < end && opt[l] != delim; ++l) {}	//Format: [delim]option (space) value (space)
			--l;										//Sets at second space / last character

			if (l == end - 1) result.push_back(string(opt + it, (l - it + 1)));		//Last value is last character
			else result.push_back(string(opt + it, (l - it)));					//Last value is second space

			it = l;

		}
		else if (isspace(opt[it])) ;			//More spaces between words in input
		else throw bad_format();					//Unexpected value

	}

	return result;

}