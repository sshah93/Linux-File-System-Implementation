/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#ifndef _NODE_H_
#define _NODE_H_

#include "header.h"

class node
{
	public:
		node()
		{

		}
		
		virtual ~node()
		{

		}

		virtual const string getName() = 0;
};

#endif