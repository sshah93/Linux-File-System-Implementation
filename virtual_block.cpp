#include "virtual_block.h"

virtual_block::virtual_block(const int& start, const int& end, const bool& free):
	start(start),
	end(end),
	free(free)
{

}

virtual_block::~virtual_block()
{

}

int virtual_block::getStart()
{
	return start;
}

void virtual_block::setStart(const unsigned int& start_range)
{
	start = start_range;
}

int virtual_block::getEnd()
{
	return end;
}

void virtual_block::setEnd(const unsigned int& end_range)
{
	end = end_range;
}

const int virtual_block::getSize()
{
	return end - start + 1;
}

const bool virtual_block::inRange(int num)
{
	if(num >= start && num <= end)
		return true;

	return false;
}

const bool virtual_block::isFree()
{
	return free;
}