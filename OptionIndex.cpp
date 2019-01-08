#include "OptionIndex.h"

vector<string>::iterator find_idx(vector<string>& target, string opt) {

	for (auto it = target.begin(); it != target.end(); ++it) {
	
		//Checks whether string at i contains wanted option
		if (it->find(opt) != string::npos) return it;  

	}

	return target.end();

}