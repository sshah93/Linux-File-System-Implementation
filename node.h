#ifndef _NODE_H_
#define _NODE_H_

#include "header.h"

bool both_are_spaces(char left, char right);

void stringSplit(const string& str, char delimiter, vector<string>& contents);

class node
{
protected:
	bool hasChildren;

public:
	node();
	virtual ~node();

	const bool has_Children();
};

#endif