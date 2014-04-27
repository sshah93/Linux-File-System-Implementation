#include "node.h"

// constructor
node::node()
{

}

// destructor
node::~node()
{

}

bool both_are_spaces(char left, char right)
{
	return (left == right) && (left == ' ');
}

// split string function which returns a new vector after splitting the entire string
void stringSplit(const string& str, char delimiter, vector<string>& contents)
{
	string individual_str;
	stringstream splitString(str);

	while(std::getline(splitString, individual_str, delimiter))
		contents.push_back(individual_str);
}