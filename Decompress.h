#pragma once
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <experimental/filesystem>
#include <fstream>
#include <string>
#include <iostream>

namespace fs = std::experimental::filesystem;

fs::path decompress(std::string&);