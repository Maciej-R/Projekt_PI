#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <exception>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Group.h"
#include "Server.h"
#include "Log.h"
#include "Send_to_Host.h"
#include "Result.h"
#include "Compress.h"

class Connection;
using namespace boost::asio;

void download_handle(User&, boost::shared_ptr<Connection>, Result&);