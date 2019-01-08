#pragma once
#include <mutex>
#include <string>
#include <algorithm>
#include <future>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "Exceptions.h"
#include "Group.h"
#include "Handle_User.h"
#include "Server.h"
#include "Adduser.h"
#include "Addgroup.h"
#include "Send_to_Host.h"
#include "Log.h"

class Connection;

void start_handling_user(boost::shared_ptr<Connection>); //Login or create user 