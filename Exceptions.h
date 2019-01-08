#pragma once
#include <stdexcept>
#include <string>

class bad_format : public std::runtime_error {

public:

	bad_format() noexcept : std::runtime_error("Wrong format") {}
	bad_format(char* ptr) noexcept : std::runtime_error(std::string(ptr)) {}
	bad_format(std::string msg) noexcept : std::runtime_error(std::move(msg)) {}

};