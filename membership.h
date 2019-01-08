#pragma once
#include "Group.h"
#include <map>

void remap_membership();

class membership{

	friend void remap_membership();
	membership();
	static std::multimap<unsigned long, Group&> memeber_of;

public:
	static void create();
	~membership();
};