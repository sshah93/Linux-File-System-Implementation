#ifndef _VIRTUAL_BLOCK_H_
#define _VIRTUAL_BLOCK_H_

#include "header.h"
#include "disk_block.h"

class virtual_block
{
protected:
	int start;
	int end;
	bool free;

public:
	virtual_block(const int& start, const int& end, const bool& free);
	virtual ~virtual_block();

	int getStart();
	void setStart(const unsigned int& start_range);

	int getEnd();
	void setEnd(const unsigned int& end_range);

	const bool isFree();
	const bool inRange(int);
	const int getSize();

	static bool compare(virtual_block* left, virtual_block* right)
	{
		return left->getStart() < right->getStart();
	}
};

#endif