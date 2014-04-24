#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include "header.h"
#include "node.h"

// inherits class node
class directory: public node
{
protected:
	// class attributes
	vector<node*> children_node;
	string dir_name;

public:
	// constructor and destructor
	directory(const string& name);
	virtual ~directory();

	// public methods
	void add_child(node*);
	void remove_chile(const string& name);
	
	// getters
	const string getName();
	const int getChildrenSize();
	const bool hasChild(const string& name);
	
	// will be used for ls
	void listChildren();
	vector<node*>* getChildren();
};

#endif
