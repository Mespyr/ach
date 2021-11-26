#include "../include/file.h"

File::File(std::string filename)
{
    file.open(filename, std::ios::out);
    if (!file)
    {
        std::cerr << "[error] can't open file '" << filename << "'" << std::endl;
        exit(1);
    }
}

File::~File()
{
    file.close();
}

void File::writeln(std::string buf)
{
    buf.push_back('\n');
    file.write(buf.c_str(), buf.length());
}
