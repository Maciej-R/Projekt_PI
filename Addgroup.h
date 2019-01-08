#pragma once
#include <mutex>
#include <vector>
#include <algorithm>
#include "Group.h"
#include "SeparateOptions.h"
#include "OptionIndex.h"
#include "OptionValue.h"
#include "Find_User.h"
#include "User_Exists.h"
#include "Send_to_Host.h"
#include "Result.h"

using std::vector;

bool addgroup(Result&, User&);