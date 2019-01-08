#pragma once
#include <vector>
#include <algorithm>
#include <experimental/filesystem>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "Server.h"
#include "Result.h"
#include "Send_to_Host.h"

void delete_command(User&, boost::shared_ptr<Connection>, Result&);