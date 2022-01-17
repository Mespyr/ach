#ifndef ILU_UTIL_H
#define ILU_UTIL_H

#include <string>

#define exit_on_error(exit_code) if (exit_code != 0) exit(exit_code);

bool is_number(std::string n);
bool is_string(std::string str);
std::string add_escapes_to_string(std::string str);

#endif
