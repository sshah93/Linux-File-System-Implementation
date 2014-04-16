#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include "header.h"

class directory: public node{

protected:
	vector<node> children_nodes;
	string dir_name;

public:
	directory(const string& name);
	virtual ~directory();

	void add_child(node*);
	const string get_name();

	vector<node*>* get_children();

};

#endif