#pragma once
#include <vector>
#include <string>
#include <algorithm>

using std::vector;
using std::string;

//Returns index of given option in vector 
//consisting of strings of separated options from input

vector<string>::iterator find_idx(vector<string>&, string);
							  //Not a reference - using temporary values