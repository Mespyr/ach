#ifndef ILU_FILE_H
#define ILU_FILE_H

#include <fstream>
#include <string.h>
#include <iostream>

class File
{
public:
    File(std::string filename);
    ~File();
    void write(std::string buf);
private:
    std::fstream file;
};

#endif
