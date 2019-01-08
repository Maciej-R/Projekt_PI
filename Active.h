#pragma once

class Active{

	static unsigned int active_;

public:

	static unsigned int active();
	static void increase();
	static void decrease();
	
};

