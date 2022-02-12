#ifndef _ACH_FILE_H_
#define _ACH_FILE_H_

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
	bool exists();
private:
	std::fstream file;
};

#endif
