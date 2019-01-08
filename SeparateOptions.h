#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "Exceptions.h"

std::vector<std::string> separate_options(char*, int, char = '-');

template<typename Pred, typename T>
std::vector<std::string> separate_options(T* opt, int end, Pred pred) {

	int it = 0;
	std::vector<std::string> result;

	for (; it < end; ++it) {		//Iterates through whole input without command name

		if (pred(opt[it])) {		//Options begin with -

			int l = ++it;
			for (; l < end && !pred(opt[l]); ++l) {}	//Format: -option ... Pred(l) == true
			if (l == it) return result;					//In case of spaces at the end
			--l;										//Sets at second space / last character

			if (l == end - 1) result.push_back(std::string(opt + it, (l - it + 1)));		//Last value is last character
			else result.push_back(std::string(opt + it, (l - it + 1)));					//Last value is second space

			it = l;

		}
		else if (isspace(opt[it])) ++it;			//More spaces between words in input
		else throw bad_format();					//Unexpected value

	}

	return result;

}
