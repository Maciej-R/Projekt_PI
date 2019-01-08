#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Group.h"
#include "Server.h"
#include "Exceptions.h"
#include "exception"
#include "Log.h"
#include "Send_to_Host.h"
#include "Result.h"

class Connection;

bool upload_handle(User&, boost::shared_ptr<Connection>, Result&);