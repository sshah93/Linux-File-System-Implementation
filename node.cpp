#include "node.h"

// constructor
node::node()
{
	this->hasChildren = false;
}

// destructor
node::~node()
{

}

// checks if the node has children
const bool node::hasChildren()
{
	return hasChildren;
}

bool doubleSpaces(char left, char right)
{
	return (left == ' ') && (left == right);
}

// split string function
void stringSplit(string& str, char delimiter, vector<string>& contents)
{
	string individual_str;
	stringstream splitString(str);

	while(getline(str, individual_str, delimiter))
		contents.push_back(individual_str);
}