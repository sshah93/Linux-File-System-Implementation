#include "file_system.h"

file_system::file_system(const unsigned int& disk_size, const unsigned int& block_size):
	disk_size(disk_size), 
	block_size(block_size),
	total_used_size(0),
  	total_fragmentation(0)
{

}

file_system::~file_system()
{

}

// adding to map
bool file_system::build_directory_structure(const vector<string>& contents, const string & unique_name)
{
	bool ret(true);

	if(root_dir.find(unique_name) != root_dir.end())
	{
		return ret;
	}

	if(contents.size() == 2)
	{
		node* add = new directory(unique_name);
		directory* pdir = static_cast<directory*>(root_dir.find("/")->second); 
		pdir->add_child(add);
		root_dir.insert(pair<string, node*>(unique_name, add));

		cout << "adding directory: " << unique_name << endl;
		cout << "adding parent directory: " << "/" << endl;

		return ret;
	}

	for(int i = contents.size()-1; i > 1; i--)
	{
		string dir(unique_name);
		string parent(unique_name);

		int pos = parent.find(contents[i]);
		parent.erase(pos, string::npos);

		if(i == contents.size()-1)
			dir = unique_name;
		
		else
		{
			pos = dir.find(contents[i-1]);
			dir.erase(pos, string::npos);
		}

		if(root_dir.find(dir) == root_dir.end())
		{
			node* add = new directory(dir);
			directory* pdir = static_cast<directory*>(root_dir.find(parent)->second); 
			pdir->add_child(add);

			root_dir.insert(pair<string, node*>(dir, add));

			cout << "adding directory: " << dir << endl;
			cout << "adding parent directory: " << parent <<endl;
		}
	}
}

// parsing the dir list
bool initialize_directories(const string& dir_list)
{

}

// parsing the file list
bool initialize_files(const string& file_list)
{

}

const void print_dir();

bool setCurrentDir(const string& file);
// for ls case
void list();
bool add_dir_under_current(const string& add, const string& unique);
bool add_file_under_current(const string& add, const string& unique);

const void bfs_file_info();
const void bfs_traverse();
const void print_blocks();
const void print_disk_info();

void remove_bytes_from_file(const string& unique, const unsigned int& bytes);
void free_blocks(vector<unsigned int>& removed_blocks);