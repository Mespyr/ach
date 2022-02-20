#ifndef _ACH_BINDTABLE_H_
#define _ACH_BINDTABLE_H_

#include <string>
#include <vector>
#include <algorithm>

class BindTable
{
public:
	std::vector<std::string> keys;

	void reg(std::vector<std::string> ks);
	long unsigned int at(std::string k);
	bool count(std::string k);
};

#endif
