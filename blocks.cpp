/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#include "blocks.h"

blocks::blocks(const int& start, const int& end, const bool& free):
	start(start),
	end(end),
	free(free)
{

}

blocks::~blocks()
{

}

int blocks::getStart()
{
	return start;
}

void blocks::setStart(const unsigned int& start_range)
{
	start = start_range;
}

int blocks::getEnd()
{
	return end;
}

void blocks::setEnd(const unsigned int& end_range)
{
	end = end_range;
}

const bool blocks::isFree()
{
	return free;
}

const bool blocks::inRange(int num)
{
	if(num >= start && num <= end)
		return true;

	return false;
}

const int blocks::getSize()
{
	return end - start + 1;
}