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

bool is_string(std::string str)
{
    if (str.back() == '"' && str.at(0) == '"')
        return true;
    return false;
}

std::string add_escapes_to_string(std::string str)
{
    std::string buf;
    std::string ret;
    int i = 0;

    while (i < str.length() - 1)
    {
        buf = str.substr(i, 2);
        if (buf == "\\a")       ret.push_back('\a');
        else if (buf == "\\b")  ret.push_back('\b');
        else if (buf == "\\f")  ret.push_back('\f');
        else if (buf == "\\n")  ret.push_back('\n');
        else if (buf == "\\r")  ret.push_back('\r');
        else if (buf == "\\t")  ret.push_back('\t');
        else if (buf == "\\v")  ret.push_back('\v');
        else if (buf == "\\\\") ret.push_back('\\');
        else if (buf == "\\'")  ret.push_back('\'');
        else if (buf == "\\\"") ret.push_back('\"');
        else if (buf == "\\\?") ret.push_back('\?');
        else 
        {
            // if escape sequence found, shift buffer over by one char to next section
            ret.push_back(buf.at(0));
            i++;
            continue;
        }

        // skip both chars if escape sequence found in buffer
        i+=2;
    }

    return ret;
}
