#pragma once
#include "Group.h"
#include "SeparateOptions.h"
#include "OptionIndex.h"
#include "OptionValue.h"
#include "Send_to_Host.h"
#include "Server.h"
#include "Find_Group.h"
#include "Result.h"
#include <boost/asio.hpp>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void manage_group(User&, boost::shared_ptr<Connection>, Result&);