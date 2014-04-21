#ifndef _FILE_H_
#define _FILE_H_

#include "header.h"
#include "node.h"
#include "disk_block.h"

class file: public node
{
protected:
	// file attributes
	string file_name;
	unsigned int file_size;
	struct tm * timestamp;

	// each file has a unique list of block addresses, each block address is mapped to a block id
	map<unsigned int, unsigned int> block_addresses;

public:
	// constructors and destructors
	file(const string& file_name, const int& file_size);
	virtual ~file();

	// converts to the block address and adds it to the list of block addresses
	void add_address(const unsigned int& block_size, const unsigned int& block_id);

	// gets the file name
	const string getName();

	// gets the file size
	const int getSize();	

	void setSize(const unsigned int& size);

	const void printInfo();

	// gets the entire map of block addresses
	std::map<unsigned int, unsigned int>* getMap();
};

#endif