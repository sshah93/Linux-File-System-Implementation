/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#ifndef _FILE_H_
#define _FILE_H_

#include "node.h"

class file: public node
{
	protected:
		// file attributes
		string mName;
		unsigned int mSize;
		tm timestamp;

		// each file has a unique list of block addresses, each block address is mapped to a block id
		// the pair is in the form of block id, block address
		map<unsigned int, unsigned int> block_addresses;

	public:
		// constructors and destructors
		file(const string& mName, const int& mSize);
		virtual ~file();

		// converts to the block address and adds it to the list of block addresses
		void new_block_address(const unsigned int& block_size, const unsigned int& block_id);

		// gets the file name
		const string getName();

		// sets the file name
		void  setName(string fname);

		// gets the file size
		const int getSize();	

		// sets the file size
		void setSize(const unsigned int& mSize);

		// gets the timestamp
		const string getTime();

		// sets the timestamp
		void setTime();

		// prints all the information about the file
		const void prfiles();

		// gets the entire map of block addresses
		std::map<unsigned int, unsigned int>* getBlockAddresses();

		// gets the last n bytes
		std::vector<int> getNBytes(int blocks);
};

#endif