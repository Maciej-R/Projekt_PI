#include "Log.h"

void to_log(std::string msg) {

	std::fstream of("file.log", std::ifstream::app);
	char date[30];
	time_t tt;// = std::time(nullptr);
	// ptt;// = new tm();
	time(&tt);
	struct tm ptt;
	localtime_s(&ptt, &tt);
	std::strftime(date, 30, "%F at %T: ", &ptt);
	
	of << date << msg << "\n";
}