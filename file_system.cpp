#include "file_system.h"


file_system::file_system(const unsigned int& disk_size, const unsigned int& block_size):
	disk_size(disk_size),
	block_size(block_size),
	total_used_size(0),
	total_fragmentation(0)
{
	current_dir = root_dir.find("/")->second;
	total_blocks = disk_size/block_size;
}

file_system::~file_system()
{
	map<string, node*>::iterator iter;
	for(iter = root_dir.begin(); iter != root_dir.end(); ++iter)
	{
		delete iter->second;
	}
	//iterate disk_blocks and delete them from memory
}

const void file_system::print_blocks()
{
	cout << "LDISK structure..." << endl;
	deque<virtual_block*>::iterator iter;
	iter = disk_blocks.begin();
	int i;
	for(; iter != disk_blocks.end() ; iter++)
	{
		cout << disk_blocks.size() << endl;
		map<int, disk_block*>* map = (*iter)->getMap();
		if(map->size() == 0)
		{
			cout << "size = 0 " << endl;
			delete *iter;
			deque<virtual_block*>::iterator it = iter;
			disk_blocks.erase(it);
		}
		else
		{
			bool free = (*iter)->isFree();
			string fr("FREE");
			if(!free)
			{
				fr = "FULL";
			}
			cout << "node: -with: " << map->size() << " contiguous " << fr << " blocks, range: " << map->begin()->first << ":" << (-- map->end())->first << endl;
			i++;
		}
	}
}

void file_system::free_blocks(vector<unsigned int> &removed_blocks)
{
	cout << "merging freed blocks..." << endl;

	vector<unsigned int>::iterator block_iter;
	block_iter = removed_blocks.begin();
	//free the removed blocks and add them to a temp vector so we can merge them correctly
	deque<disk_block*> removedb;
	cout << "removed block id's:  ";

	for(; block_iter != removed_blocks.end(); block_iter++)
	{
		deque<virtual_block*>::iterator virtual_iter;
		virtual_iter = disk_blocks.begin();

		for(; virtual_iter != disk_blocks.end() ; virtual_iter++)
		{
			if(!(*virtual_iter)->isFree())
			{
				map<int, disk_block*>* map = (*virtual_iter)->getMap();

				if(map->find(*block_iter) != map->end())
				{
					disk_block* removed = map->find(*block_iter)->second;
					removedb.push_back(removed);
					map->erase(*block_iter);
					cout << "-" << *block_iter;
					break;
				}
			}
		}
	}
	cout << endl;

	//merge blocks if needed
	deque<disk_block*>::reverse_iterator iter;
	iter = removedb.rbegin();
	while(iter != removedb.rend())
	{
		cout << "LOOP" << (*iter)->getBlockID() << endl;
		deque<virtual_block*>::iterator virtual_iter;
		virtual_iter = disk_blocks.begin();

		for(; virtual_iter != disk_blocks.end() ; virtual_iter++)
		{
			map<int, disk_block*>* map = (*virtual_iter)->getMap();
			if(map->size() == 0)
			{
				continue;
			}
			else if((map->begin()->first < (*iter)->getBlockID()) && ((map->end()--)->first > (*iter)->getBlockID()))
			{
				cout << "RANGE: " << map->begin()->first << ":" << (map->end()--)->first << endl;
				cout  << (*iter)->getBlockID() << endl;

				if((*virtual_iter)->isFree())
				{
					cout << "no need to merge" << endl;
					map->insert(pair<int, disk_block*>((*iter)->getBlockID(), *iter));
					break;
				}
				else
				{
					map<int, disk_block*>* 

					new_map = 
					new 
					map<int, disk_block*>();
					new_map->insert(pair<int, disk_block*>((*iter)->getBlockID(), *iter));

					map<int, disk_block*>::iterator upper_bound = map->upper_bound((*iter)->getBlockID());
					for(;upper_bound != map->end(); upper_bound++)
					{
						new_map->insert(pair<int, disk_block*>(upper_bound->first, upper_bound->second));
						cout << "upper bound: " << upper_bound->first << endl;
					}

					upper_bound = map->upper_bound((*iter)->getBlockID());
					for(;upper_bound != map->end(); upper_bound++)
					{
						map->erase(upper_bound);
					}

					virtual_block* add = new virtual_block(true);
					add->replaceList(new_map);
					disk_blocks.push_back(add);
					cout <<  disk_blocks.size() << endl;
					break;
				}
			}
		}
		iter++;
	}
}

void file_system::remove_bytes_from_file(const string &unique, const unsigned int& bytes)
{
	directory* pdir = static_cast<directory*>(current_dir); 
	vector<node*>* children = pdir->get_children();
	for(unsigned int i = 0; i < children->size(); i++)
	{
		file* pfile = dynamic_cast<file*>((*children)[i]);
		if(pfile)
		{
			if(pfile->getName() == unique)
			{
				map<unsigned int, unsigned int>* blocks = pfile->getMap();
				//vector to keep track of the blocks that were just freed from the file
				vector<unsigned int> removed_blocks;
				map<unsigned int, unsigned int>::reverse_iterator iter = blocks->rbegin();
				unsigned int size_removed = 0;
				for(; iter != blocks->rend();)
				{
					removed_blocks.push_back(iter->second);
					blocks->erase( --(iter.base()) );
					size_removed = size_removed + block_size;
					if((size_removed >= bytes) || (size_removed >= (pfile->get_size())))
					{ 
						//TODO fix fragmentation
						if(size_removed >= (pfile->get_size()))
						{
							size_removed = pfile->get_size();
						}
						cout << "removed " << size_removed << " bytes, and " << (size_removed+1)/block_size << " blocks from: " << pfile->getName() << endl;
						pfile->set_size(pfile->get_size() - size_removed);
						//call function and pass vector to remove freed blocks and merge them if possible
						free_blocks(removed_blocks);
						break;
					}
					else
					{
						iter = blocks->rbegin();
					}
				}
			}
		}
	}
}

//this always assumes that there is free space avilable, whoever calls this needs to make sure, space is avaailable
bool file_system::move_blocks_to_free_node(vector<disk_block*> &blocks )
{
	bool ret(false);
	for(unsigned int i = 0; i < disk_blocks.size(); i++)
	{
		bool free = disk_blocks.at(i)->isFree();
		if(!free)
		{
			map<int, disk_block*>* map = disk_blocks.at(i)->getMap();
			for(unsigned int i =0; i < blocks.size(); i++)
			{
				map->insert(pair<int, disk_block*>(blocks[i]->getBlockID(), blocks[i]));
			}
			ret = true;
		}
	}
	if(!ret)
	{
		map<int, disk_block*>* virtual_node = new map<int, disk_block*>();
		for(unsigned int i = 0; i < blocks.size(); i++)
		{
			virtual_node->insert(pair<int, disk_block*>(blocks[i]->getBlockID(), blocks[i]));
		}
		ret = true;
		virtual_block* add = new virtual_block(false);
		add->replaceList(virtual_node);
		disk_blocks.push_back(add);
	}
	return ret;
}

//file would like x amount of space, see if we can add it
//Sorry for this nasty algorithm
bool file_system::handle_file_request(file* file, const unsigned int& space_requested)
{
	bool ret(false);
	if(total_used_size >= disk_size)
	{
		cout << "ERROR not enough space available" << endl;
		return ret;
	}
	for(unsigned int i = 0; i < disk_blocks.size(); i++)
	{
		if(disk_blocks.at(i)->isFree())
		{
#ifdef DEBUG
			cout << "allocating blocks for file " << file->getName() << " total size: " << space_requested << endl;
#endif
			map<int, disk_block*>* map = disk_blocks.at(i)->getMap();
			if(map->size() * block_size < space_requested)
			{
				continue;
			}

			vector<disk_block*> curr_blocks;
			map<int, disk_block*>::iterator iter;
			iter = map->begin();
			unsigned int size_found = 0;
			for(; iter != map->end(); iter++)
			{
				curr_blocks.push_back(iter->second);
				map->erase(iter);

				size_found = size_found + block_size;
				if(size_found >= space_requested)
				{ 
					total_fragmentation = total_fragmentation + (size_found-space_requested);
					total_used_size = total_used_size + size_found;
#ifdef DEBUG
					cout << curr_blocks.size() << " blocks allocated for file: " << file->getName() << endl;
#endif
					for(unsigned int i =0; i < curr_blocks.size(); i++)
					{
						file->add_address(block_size, curr_blocks[i]->getBlockID());
					}
					if(!move_blocks_to_free_node(curr_blocks))
					{
						cout << "error cannot allocate blocks" << endl;
						return ret;
					}
					ret = true;
					break;
				}
			}
		}
	}
	return ret;
}

//build Ldisk starting with all blocks as one contiguous virtual_block since they are all free.
bool file_system::build_file_structure()
{
	bool ret(false);
	map<int, disk_block*>* virtual_node = new map<int, disk_block*>();
	unsigned int curr_block_id = 0;
	cout << "Adding initial blocks, total blocks: " << total_blocks << endl << "This takes a couple seconds ... " << endl;
	while(total_blocks > virtual_node->size())
	{
		disk_block* block = new disk_block(block_size, curr_block_id);
		virtual_node->insert(pair<int, disk_block*>(curr_block_id, block));
		curr_block_id++;
	}
	ret = true;
	virtual_block* add = new virtual_block(true);
	add->replaceList(virtual_node);
	disk_blocks.push_back(add);
	return ret;
}


bool file_system::build_directory_structure(const vector<string>& contents, const string& unique_name)
{
	bool ret(true);

	if(root_dir.find(unique_name) != root_dir.end())
	{
		return ret;
	}

	if(contents.size() == 2)
	{
		ret = true;
		node* add = new directory(unique_name);
		directory* pdir = static_cast<directory*>(root_dir.find("/")->second); 
		pdir->add_child(add);
		root_dir.insert(pair<string, node*>(unique_name, add));
#ifdef DEBUG
		cout << "adding directory: " << unique_name << endl;
		cout << "adding parent directory: " << "/" << endl;
#endif
		return ret;
	}

	ret = true;
	for(unsigned int i = contents.size()-1; i > 1; i--)
	{
		string dir(unique_name);
		string parent(unique_name);

		unsigned int pos = parent.find("/" + contents[i]);
		parent.erase(pos, string::npos);
		if(i == contents.size()-1)
		{
			dir = unique_name;
		}
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

#ifdef DEBUG
			cout << "adding directory: " << dir << endl;
			cout << "of parent directory: " << parent <<endl;
#endif
			break;
		}
	}
	return ret;
}

bool file_system::initialize_directories(const string &file_name)
{
	build_file_structure();
	bool ret(false);
	string line;
	ifstream directory_list(file_name.c_str(), ios::in);
	cout << "opening file ... " << file_name << endl;

	//add default root directory
	node* add = new directory("/");
	root_dir.insert(pair<string, node*>("/", add));

	if(directory_list.is_open())
	{
		while(getline(directory_list, line))
		{
			if(line.size() <= 3)
			{
				continue;
			}
			vector<string> contents;
			split(line, '/', contents);
			line.erase(line.begin());
#ifdef DEBUG
			cout << "LINE READ: " << line << endl;
#endif
			build_directory_structure(contents, line);

		}
		ret = true;
	}
	else
	{
		cout << "ERROR: Unable to open file: " << file_name << endl;
	}
	return ret;
}

bool file_system::initialize_files(const string &file_name)
{
	bool ret(false);
	string line;
	ifstream file_list(file_name.c_str(), ios::in);
	cout << "opening file ... " << file_name << endl;

	if(file_list.is_open())
	{
		while(getline(file_list, line))
		{
			print_blocks();
			if(line.size() <= 3)
			{
				continue;
			}
			vector<string> contents;
			string::iterator new_end = unique(line.begin(), line.end(), both_are_spaces);

			//remove unessary spaces, and split line into spaces
			line.erase(new_end, line.end());
			split(line, ' ', contents);
#ifdef DEBUG
			cout << "file read: " << contents[10] << " timestamp: " << contents[9] << "  size:" << contents[6] << endl;
#endif
			string file_name(contents[10]);
			int pos = file_name.rfind("/");
			file_name.erase(0, pos+1);

			string parent(contents[10]);
			parent.erase(pos, string::npos);
			parent.erase(0, 1);
			if(parent.compare("") == 0)
			{
				parent = "/";
			}
			if(root_dir.find(parent) != root_dir.end())
			{
				string fsize(contents[6]);
				istringstream buffer(fsize);
				int size;
				buffer >> size;
				directory* pdir = static_cast<directory*>(root_dir.find(parent)->second);
				node* file = new file(file_name, size);
				pdir->add_child(file);
				file* pfile = static_cast<file*>(file);
				if(!handle_file_request(pfile, size))
				{
					cout << "error not enough free space to create blocks for file : " << file_name << endl;
					return false;
				}
			}
			else
			{
				cout << "error directory doesn't exist" << endl;
			}

#ifdef DEBUG
			cout << "file added: " << file_name << endl;
			cout << "to parent directory " << parent << endl;
#endif
		}
		ret = true;

	}
	else
	{
		cout << "ERROR: Unable to open file: " << file_name << endl;
	}
	return ret;
}

const void file_system::print_directory()
{
	print_directory(root_dir.find("/")->second);
}

const void file_system::bfs_file_info()
{
	queue<node*> bfs;
	node* current(root_dir.find("/")->second);
	bfs.push(current);

	while(!bfs.empty())
	{
		current = bfs.front();
		directory* pdir = dynamic_cast<directory*>(current);
		if(pdir)
		{
			vector<node*>* children = pdir->get_children();
			for(unsigned int i = 0; i < children->size(); i++)
			{
				node* node = dynamic_cast<node*>(children->at(i));
				if(!node)
				{
					cout << "error cannot cast to node..." << endl;
					return;
				}
				bfs.push(node);
			}
		}
		else
		{
			file* file = dynamic_cast<file*>(current);
			file->print_info();
		}
		bfs.pop();
	}
}

const void file_system::bfs_traverse()
{
	queue<node*> bfs;
	node* current(root_dir.find("/")->second);
	bfs.push(current);

	while(!bfs.empty())
	{
		current = bfs.front();
		directory* pdir = dynamic_cast<directory*>(current);
		if(pdir)
		{
			cout << "Directory: " << pdir->getName() << endl;
			vector<node*>* children = pdir->get_children();
			for(unsigned int i = 0; i < children->size(); i++)
			{
				node* node = dynamic_cast<node*>(children->at(i));
				if(!node)
				{
					cout << "error cannot cast to node..." << endl;
					return;
				}
				bfs.push(node);
			}
		}
		else
		{
			file* file = dynamic_cast<file*>(current);
			cout << "File: " << file->getName() << endl;
		}
		bfs.pop();
	}
}

const void file_system::print_directory(node* root)
{
	directory* pdir = dynamic_cast<directory*>(root);
	if(pdir)
	{
		cout << "Directory: " << pdir->getName() << endl;
		vector<node*>* children = pdir->get_children();
		for(unsigned int i = 0; i < children->size(); i++)
		{
			node* node = dynamic_cast<node*>(children->at(i));
			if(!node)
			{
				cout << "error cannot cast to node..." << endl;
			}
			print_directory(node);
		}
	}
	else
	{
		file* file = dynamic_cast<file*>(root);
		if(file)
		{
			cout << file->getName() << endl;
		}
	}
}

bool file_system::setCurrentDir(const string & file)
{
	bool ret(false);
	if(root_dir.find(file) != root_dir.end())
	{
		current_dir = root_dir.find(file)->second;
		ret = true;
	}
	return ret;
}

void file_system::list()
{
	directory* cur = dynamic_cast<directory*>(current_dir);
	cur->list();
}


bool file_system::add_dir_under_current(const string &add, const string &unique)
{
	bool ret(false);
	if(root_dir.find(unique) != root_dir.end())
	{
		ret = true;
		directory* cur = dynamic_cast<directory*>(current_dir);
		node* new_dir = new directory(add);
		cur->add_child(new_dir);
		root_dir.insert(pair<string, node*>(unique, new_dir));
	}
	return ret;
}

bool file_system::add_file_under_current(const string &add, const string &unique)
{
	bool ret(false);
	if(root_dir.find(unique) != root_dir.end())
	{
		ret = true;
		directory* cur = dynamic_cast<directory*>(current_dir);
		node* new_file = new file(add, 0);
		cur->add_child(new_file);
		root_dir.insert(pair<string, node*>(unique, new_file));
	}
	return ret;
}

const void file_system::print_disk_info()
{
	cout << "total disk size: " <<  disk_size << " -total blocks: " << total_blocks << "- total used space (including fragmentation): " << 
		total_used_size  << " - fragmentation: " << total_fragmentation << endl;
	print_blocks();
}