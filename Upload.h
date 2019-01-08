#pragma once
#include <filesystem>
#include <vector>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Connection.h"

namespace fs = std::experimental::filesystem;
using std::vector;

void upload_file(fs::path&, boost::shared_ptr<Connection>);