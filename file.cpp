#include "file.h"

file::file(const string& fname, const int& size):
	node(),
	file_name(fname),
	file_size(size)
{
	time_t t = time(0);
	timestamp = localtime(&t);
}

file::~file()
{

}

const string file::getName()
{
	return file_name;
}

void setName(string fname)
{
	string name = fname;
}

const int file::getSize()
{
	return file_size;
}

map<unsigned int, unsigned int>* file::getMap()
{
	return &block_addresses;
}

void file::setSize(const unsigned int& mSize)
{
	file_size = mSize;
}

void file::add_address(const unsigned int& block_size, const unsigned int& block_id)
{
	unsigned int address = block_id * block_size;
	block_addresses.insert(pair<unsigned int, unsigned int>(address, block_id));
}

const void file::printInfo()
{
	cout << "file: " << file_name << "size: " << file_size << " total blocks used: " << block_addresses.size() << endl;
	cout << "block addresses: " << endl;

	map<unsigned int, unsigned int>::iterator iter = block_addresses.begin();
	
	for(; iter != block_addresses.end(); iter++)
		cout << iter->first << "-";
	cout << endl;
}