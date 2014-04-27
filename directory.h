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
		void addChild(node*);

		// helper function for remove case
		void removeChild(const string& name);

		// getters
		const string getName();
		const int getChildrenSize();

		// checks if the children_node of a directory is empty 
		const bool hasChild(const string& name);

		// will be used for ls
		void listChildren();

		// helper function for cd
		vector<node*>* getChildren();
};

#endif