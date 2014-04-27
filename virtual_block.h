#ifndef _VIRTUAL_BLOCK_H_
#define _VIRTUAL_BLOCK_H_

#include "header.h"

class virtual_block
{
	protected:
		// class attributes
		int start;
		int end;
		bool free;

	public:
		virtual_block(const int& start, const int& end, const bool& free);
		virtual ~virtual_block();

		// getters and setters for start
		int getStart();
		void setStart(const unsigned int& start_range);

		// getters and setters for end
		int getEnd();
		void setEnd(const unsigned int& end_range);

		// helper functions 
		const bool isFree();
		const bool inRange(int);
		const int getSize();

		// function to compare 2 virtual blocks
		static bool compare(virtual_block* left, virtual_block* right)
		{
			return left->getStart() < right->getStart();
		}
};

#endif
