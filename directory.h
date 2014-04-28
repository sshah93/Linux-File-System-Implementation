/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include "node.h"

// helper functions that will be used in the entire project
void stringSplit(const string& str, char delimiter, vector<string>& contents);
bool both_are_spaces(char left, char right);

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