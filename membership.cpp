#include "membership.h"

extern std::vector<Group> Groups_Global;
std::multimap<unsigned long, Group&> membership::memeber_of;

membership::membership()
{
}


void remap_membership()
{
	for (auto x : Groups_Global) {
	
		for (auto y : x.members) {
			
			//Project User ID on groups that's he's a member of
			membership::memeber_of.emplace(std::pair<unsigned long, Group&>(y.ID(), x));
		
		}

		for (auto y : x.owners) {

			//For owners
			membership::memeber_of.emplace(std::pair<unsigned long, Group&>(y.ID(), x));

		}
	
	}

}

void membership::create(){

	remap_membership();

}

membership::~membership()
{
}
