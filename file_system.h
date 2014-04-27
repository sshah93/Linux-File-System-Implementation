#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "header.h"
#include "node.h"
#include "directory.h"
#include "file.h"
#include "virtual_block.h"

class file_system
{
protected:
	unsigned int disk_size;
	unsigned int block_size;
	unsigned int total_blocks;
	unsigned int total_used_size;
	unsigned int total_fragmentation;

	node* current_dir;
	map<string, node*> root_dir;
	deque<virtual_block*> disk_blocks;

	// creating the directory structure
	bool build_directory_structure(const vector<string>& contents, const string& unique_name);
	
	// creating the file structure
	bool build_file_structure();

	// bool move_blocks_to_free_node(vector<disk_block*> &blocks);

	// deletes range of virtual blocks
	void delete_range(int start, int end);

	// merges 2 empty/closed blocks together
	void merge();

	// updates from a file
	bool handle_file_request(file* file, const unsigned int& space_requested);

	// dfs 
	const void print_directory(node* root);

	// helper function to find a file
	file* find_file(const string& unique_name);

public:

	file_system(const unsigned int& disk_size, const unsigned int& block_size);
	virtual ~file_system();

	bool initialize_directories(const string& dir_list);
	bool initialize_files(const string& file_list);

	const void print_directory();

	bool setCurrentDir(const string& file);
	void list();
	void remove(const string& file);
	bool add_dir_under_current(const string& add, const string& unique);
	bool add_file_under_current(const string& add, const string& unique);

	const void bfs_file_info();
	const void bfs_traverse();
	const void print_blocks();
	const void print_disk_info();

	void remove_bytes_from_file(const string& unique, const unsigned int& bytes);
	void add_bytes_to_file(const string& unique, const unsigned int& bytes);
};

#endif