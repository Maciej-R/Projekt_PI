#pragma once
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

//Function compresses file, saves it in directory
//for temporary files and returns path to it
fs::path compress(fs::path&);