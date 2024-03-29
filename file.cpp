/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#include "file.h"

file::file(const string& fname, const int& size):
	node(),
	mName(fname),
	mSize(size)
{
	time_t t = time(0);
	timestamp = *localtime(&t);
}

file::~file()
{

}

void file::new_block_address(const unsigned int& block_size, const unsigned int& block_id)
{
	unsigned int address = block_size * block_id;
	block_addresses.insert(pair<unsigned int, unsigned int>(block_id, address));
}

const string file::getName()
{
	return mName;
}

void setName(string fname)
{
	string name = fname;
}

const int file::getSize()
{
	return mSize;
}

void file::setSize(const unsigned int& size)
{
	mSize = size;
}

const string file::getTime()
{
	stringstream s;
	s << (timestamp.tm_hour) << ":" << (timestamp.tm_min) << ":" << timestamp.tm_sec << "  " << (timestamp.tm_year + 1900) << '-'  << (timestamp.tm_mon + 1) << '-' <<  timestamp.tm_mday;
	return s.str();
}

void file::setTime()
{
	time_t t = time(0);
	timestamp = *localtime(&t);
}

const void file::prfiles()
{
	cout << "File: " << mName << endl;
	cout << "Size: " << mSize << endl;
	cout << "Total # of blocks used: " << block_addresses.size() << endl;
	cout << "block addresses: " << endl;

	map<unsigned int, unsigned int>::iterator iter = block_addresses.begin();

	for(; iter != block_addresses.end(); iter++)
		cout << iter->second << "-";
	cout << endl << endl;
}

map<unsigned int, unsigned int>* file::getBlockAddresses()
{
	return &block_addresses;
}

vector<int> file::getNBytes(int blocks)
{
	vector<int> result;
	int num = blocks;

	if(!num)
		return result;

	map<unsigned int, unsigned int>::reverse_iterator iter = block_addresses.rbegin();

	unsigned int end = iter->first;
	unsigned int last = end;

	block_addresses.erase(--iter.base());
	num--;

	while(iter != block_addresses.rend() && num > 0)
	{
		if(--last == iter->first)
		{
			block_addresses.erase(--iter.base());
			continue;
		}

		else
		{
			result.push_back(end);
			result.push_back(last);

			end = iter->first;
			last = end;

			block_addresses.erase(--iter.base());
		}

		num--;
	}

	result.push_back(end);
	result.push_back(last);
	return result;
}
