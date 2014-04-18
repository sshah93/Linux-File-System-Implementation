#ifndef _FILE_H_
#define _FILE_H_

#include "header.h"

class file: public node{

protected:
	deque<disk_block> block_addresses;

	string file_name;
	unsigned int file_size;
	struct tm * timestamp;

public:
	file(const string& file_name, const int& file_size);
	virtual file();

};

#endif
