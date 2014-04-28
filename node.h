/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#ifndef _NODE_H_
#define _NODE_H_

#include "header.h"

// helper functions that will be used in the entire project
void stringSplit(const string& str, char delimiter, vector<string>& contents);
bool both_are_spaces(char left, char right);

class node
{
	public:
		node();
		virtual ~node();

		virtual const string getName() = 0;
};

#endif