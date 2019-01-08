#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <filesystem>
#include <regex>
#include <sstream>
#include <memory>
#include <mutex>
#include <chrono>
#include <cstdio>
#include <iterator>
#include <boost/asio.hpp>
#include <boost/asio/ip/network_v4.hpp>
#include "Separate_Options.h"
#include "Connection.h"
#include "Receive_Data.h"
#include "Upload.h"
#include "Download.h"
#include "Result.h"
#include "Config.h"

using namespace boost::asio;
namespace fs = std::experimental::filesystem;
using std::cin;
using std::cout;
	
bool connect();