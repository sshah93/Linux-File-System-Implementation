#ifndef _DISK_BLOCK_H_
#define _DISK_BLOCK_H_

#include "header.h"

class disk_block
{
protected:
	// class attributes
	unsigned int block_address;
	unsigned int block_size;
	unsigned int block_id;

public:
	// constructor
	disk_block(const unsigned int blockAddress, const unsigned int blockSize, const unsigned int blockID);
	
	// destructor
	virtual ~disk_block();

	// getters and setters for the class attributes
	const int getBlockAddress();
	void setBlockAddress(int bAddress);
	const int getBlockSize();	
	void setBlockSize(int bSize);
	const int getBlockID();
	void setBlockID(int bID);
};

#endif