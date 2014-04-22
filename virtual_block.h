#ifndef _VIRTUAL_BLOCK_H_
#define _VIRTUAL_BLOCK_H_

#include "header.h"
#include "disk_block.h"

class virtual_block
{
protected:
	map<int, disk_block*>* Map;
	bool free;

public:
	virtual_block(const bool& free);
	virtual ~virtual_block();

	void replaceList(map<int, disk_block*> *m);
	map<int, disk_block*>* getMap();

	bool addSpace(const unsigned int& size, const unsigned int& block_size);
	const bool isFree();
};

#endif