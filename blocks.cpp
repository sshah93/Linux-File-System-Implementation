/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#include "blocks.h"

blocks::blocks(const int& start, const int& end, const bool& free):
	mStart(start),
	mEnd(end),
	mFree(free)
{

}

blocks::~blocks()
{

}

int blocks::getStart()
{
	return mStart;
}

void blocks::setStart(const unsigned int& start_range)
{
	mStart = start_range;
}

int blocks::getEnd()
{
	return mEnd;
}

void blocks::setEnd(const unsigned int& end_range)
{
	mEnd = end_range;
}

const bool blocks::isEmpty()
{
	return mFree;
}

const bool blocks::inLine(int num)
{
	if(num >= mStart && num <= mEnd)
		return true;

	return false;
}

const int blocks::blockSz()
{
	return mEnd - mStart + 1;
}