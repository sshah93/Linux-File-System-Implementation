#ifndef _NODE_H_
#define _NODE_H_

#include "header.h"

// helper functions
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