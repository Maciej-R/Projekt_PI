#pragma once
#include <mutex>
#include <string>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "Exceptions.h"
#include "Group.h"
#include "Server.h"
#include "Addgroup.h"
#include "Upload.h"
#include "Log.h"
#include "Manage_Group.h"
#include "LS.h"
#include "Result.h"
#include "Download.h"
#include "Active.h"
#include "Mkdir.h"
#include "Delete.h"

class Connection;

void handle_user(User&, boost::shared_ptr<Connection>); //Normal work