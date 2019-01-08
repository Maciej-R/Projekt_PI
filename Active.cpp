#include "Active.h"

unsigned int Active::active_ = 0;

unsigned int Active::active(){

	return active_;

}

void Active::increase(){

	++active_;

}

void Active::decrease(){

	if (active_) --active_;

}
