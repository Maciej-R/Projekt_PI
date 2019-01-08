#include "OptionValue.h"

std::string option_value(std::string& opt) {

	auto x = std::find_if(opt.cbegin(), opt.cend(), [](char c) { return isspace(c); });	//Skips option indicator
	if (x == opt.cend()) return std::string{};

	return std::string(++x, opt.cend());

}