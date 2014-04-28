/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include "node.h"
#include "directory.h"
#include "file.h"
#include "blocks.h"

class file_system
{
	protected:
		// class attributes
		unsigned int disk_size;
		unsigned int block_size;
		unsigned int total_blocks;
		unsigned int total_used_size;
		unsigned int total_fragmentation;

		node* current_dir;
		map<string, node*> root_dir;
		deque<blocks*> disk_blocks;

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
		// constructor and destructor
		file_system(const unsigned int& disk_size, const unsigned int& block_size);
		virtual ~file_system();

		// used for creating the tree structure using the initial files provided
		bool initialize_directories(const string& dir_list);
		bool initialize_files(const string& file_list);

		// used to test dfs
		const void print_directory();

		// helper function for cd case
		bool setCurrentDir(const string& file);

		// helper function for ls case
		void list();

		// helper function for delete case
		void remove(const string& file);

		// helper function for mkdir case
		bool add_dir_under_current(const string& add, const string& unique);

		// helper function for create case
		bool add_file_under_current(const string& add, const string& unique);

		// helper function for bfs_traverse function
		const void bfs_file_info();

		// helper function for dir case
		const void bfs_traverse();

		// helper function for prfiles case
		const void print_blocks();

		// helper function for prdisk case
		const void print_disk_info();

		// helper function for removing bytes from a file case
		void remove_bytes_from_file(const string& unique, const unsigned int& bytes);

		// helper function for adding bytes to a file case
		void add_bytes_to_file(const string& unique, const unsigned int& bytes);
};

#endif