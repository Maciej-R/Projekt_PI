#pragma once
#include <vector>
#include <algorithm>
#include <chrono>
#include <experimental/filesystem>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "Server.h"
#include "Result.h"
#include "Send_to_Host.h"
#include "OptionIndex.h"

namespace fs = std::experimental::filesystem;

void ls(User&, boost::shared_ptr<Connection>, Result&);