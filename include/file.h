#ifndef ILU_FILE_H
#define ILU_FILE_H

#include "util.h"
#include <fstream>
#include <string>
#include <iostream>

enum MODE
{
    MODE_READ,
    MODE_WRITE
};

class File
{
public:
    File(std::string filename, MODE mode);
    ~File();
    void writeln(std::string buf);
    std::string read();
private:
    std::fstream file;
};

#endif
