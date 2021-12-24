#ifndef ILU_LOCATION_H
#define ILU_LOCATION_H

#include <string>

class Location
{
public:
    Location(int line_number, int column_number_start, int column_number_end, std::string line, std::string file_location) :
        line_number(line_number), 
        column_number_start(column_number_start), 
        column_number_end(column_number_end),
        line(line),
        file_location(file_location)
    {};

    int line_number, column_number_start, column_number_end;
    std::string line, file_location;
};

#endif
