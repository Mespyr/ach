#ifndef ILU_UTIL_H
#define ILU_UTIL_H

#include <string>

#define ERROR_COLOR "\e[1;31m"
#define INFO_COLOR  "\e[1;32m"
#define RESET_COLOR "\e[0m"
#define exit_on_error(exit_code) if (exit_code) exit(exit_code);

bool is_number(std::string n);
bool is_string(std::string str);
std::string add_escapes_to_string(std::string str);

#endif
