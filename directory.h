#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include "header.h"

class directory: public node
{
protected:
	vector<node> children_node;
	string dir_name;

public:
	directory(const string& name);
	virtual ~directory();

	void add_child(node*);
	const string getName();
	void listChildren();

	vector<node*>* getChildren();
};

#endif