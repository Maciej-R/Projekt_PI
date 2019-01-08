#pragma once
#include <filesystem>
#include <vector>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Connection.h"
#include "Config.h"

namespace fs = std::experimental::filesystem;
using std::vector;

void download_file(boost::shared_ptr<Connection>, std::string&);