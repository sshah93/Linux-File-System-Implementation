/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#ifndef _BLOCKS_H_
#define _BLOCKS_H_

#include "header.h"

class blocks
{
	protected:
		// class attributes
		int start;
		int end;
		bool free;

	public:
		// constructors and destructors
		blocks(const int& start, const int& end, const bool& free);
		virtual ~blocks();

		// getters and setters for start
		int getStart();
		void setStart(const unsigned int& start_range);

		// getters and setters for end
		int getEnd();
		void setEnd(const unsigned int& end_range);

		// helper functions 
		// checks if the virtual block is free or not
		const bool isFree();

		// checks if the block that we need is enough 
		const bool inRange(int);

		// calculates the size of the virtual block
		const int getSize();

		// function to compare 2 virtual blocks
		static bool compare(blocks* left, blocks* right)
		{
			return left->getStart() < right->getStart();
		}
};

#endif