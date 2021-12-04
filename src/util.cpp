#include "../include/util.h"

bool is_number(std::string n)
{
    if (n.at(0) == '-')
        n = n.substr(1);

    std::string::const_iterator it = n.begin();
    while (it != n.end() && std::isdigit(*it)) 
        ++it;

    return !n.empty() && it == n.end();
}
