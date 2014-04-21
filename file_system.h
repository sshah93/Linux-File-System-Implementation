#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "node.h"
#include "directory.h"
#include "disk_block.h"

class file_system{

protected:
	map<string, node*> root_dir;
	deque<disk_block> block_addresses;

	unsigned int disk_size;
	unsigned int block_size;
	unsigned int total_blocks;
	unsigned int total_used_size;
	unsigned int total_fragmentation;

	// creating the directory structure
	bool build_directory_structure(const std::vector<string>& contents, const string& unique_name);
	
	// creating the file structure
	bool build_file_structure();

	bool handle_file_request(file* file, const unsigned int& space_requested);

	// dfs 
	const void print_dir(node* root);

public:

	file_system(const unsigned int& disk_size, const unsigned int& block_size);
	virtual ~file_system();

	bool initialize_directories(const string& dir_list);
	bool initialize_files(const string& file_list);
};

#endif