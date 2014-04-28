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
		bool dir_struct(const vector<string>& contents, const string& unique_name);

		// creating the file structure
		bool file_struct();

		// helper function to delete a directory
		void deleteDir(const string& name);

		// deletes range of virtual blocks
		void remove_virtual_blocks_range(int start, int end);

		// merges 2 empty/closed blocks together
		void join_virtual_blocks();

		// updates from a file
		bool handle_file_request(file* file, const unsigned int& space_requested);

		// dfs 
		const void printDir(node* root);

		// helper function to find a file
		file* fileLookup(const string& unique_name);

	public:
		// constructors and destructors
		file_system(const unsigned int& disk_size, const unsigned int& block_size);
		virtual ~file_system();

		// used for creating the tree structure using the initial files provided
		bool init_dirs(const string& dir_list);
		bool init_files(const string& file_list);

		// used to test dfs
		const void printDir();

		// helper function for cd case
		bool setCurrentDir(const string& file);

		// helper function for ls case
		void ls();

		// helper function for delete case
		void removeFile(const string& file);

		// helper function for mkdir case
		bool newDir(const string& add, const string& unique);

		// helper function for create case
		bool newFile(const string& add, const string& unique);

		// helper function for bfs_traverse function
		const void bfsFileInfo();

		// helper function for dir case
		const void bfsTraverse();

		// helper function for prfiles case
		const void printBlocks();

		// helper function for prdisk case
		const void printDiskInfo();

		// helper function for removing bytes from a file case
		void takeAwayBytes(const string& unique, const unsigned int& bytes);

		// helper function for adding bytes to a file case
		void giveBytes(const string& unique, const unsigned int& bytes);
};

#endif
