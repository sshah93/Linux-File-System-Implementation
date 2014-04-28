/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#ifndef _NODE_H_
#define _NODE_H_

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <utility>
#include <math.h>
#include <queue>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <time.h>
#include <ctime>
#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

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