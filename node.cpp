#include "node.h"

// constructor
node::node():hasChildren(false)
{
	
}

// destructor
node::~node()
{

}

// checks if the node has children
const bool node::has_Children()
{
	return hasChildren;
}

bool doubleSpaces(char left, char right)
{
	return (left == ' ') && (left == right);
}

// split string function which returns a new vector after splitting the entire string
void stringSplit(const string& str, char delimiter, vector<string>& contents)
{
	string individual_str;
	stringstream splitString(str);

	while(std::getline(splitString, individual_str, delimiter))
		contents.push_back(individual_str);
}
