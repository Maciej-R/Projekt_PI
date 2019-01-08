#pragma once
#include <vector>
#include <algorithm>
#include <experimental/filesystem>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "Server.h"
#include "Result.h"
#include "Send_to_Host.h"

namespace fs = std::experimental::filesystem;

void mkdir(User&, boost::shared_ptr<Connection>, Result&); 
void mkdir(Group&, boost::shared_ptr<Connection>, Result&);