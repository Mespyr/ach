#include "../include/bindtable.h"

void BindTable::reg(std::vector<std::string> ks)
{
	std::reverse(ks.begin(), ks.end());
	std::reverse(keys.begin(), keys.end());

	for (std::string k : ks)
	{
		// if k in keys, get index of value and remove it
		if (count(k))
			keys.erase(keys.begin() + at(k));
		keys.push_back(k);
	}

	std::reverse(keys.begin(), keys.end());
}

bool BindTable::count(std::string k)
{
	for (std::string ck : keys)
		if (ck == k) return true;
	return false;
};

long unsigned int BindTable::at(std::string k)
{
	for (long unsigned int i = 0; i < keys.size(); i++)
		if (keys.at(i) == k) return i;
	exit(1);
}
