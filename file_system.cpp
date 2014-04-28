/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#include "file_system.h"

/************************ Class protected helper functions ***********************************/

bool file_system::dir_struct(const vector<string>& contents, const string& unique_name)
{
	bool ret = true;

	if(root_dir.find(unique_name) != root_dir.end())
		return ret;

	if(contents.size() == 2)
	{
		ret = true;
		node* add = new directory(unique_name);
		directory* parent = static_cast<directory*>(root_dir.find("/")->second); 
		parent->addChild(add);
		root_dir.insert(pair<string, node*>(unique_name, add));

		cout << "adding directory: " << unique_name << endl;
		// cout << "adding parent directory: " << "/" << endl;

		return ret;
	}

	ret = true;

	for(unsigned int i = contents.size()-1; i > 1; i--)
	{
		string dir = unique_name;
		string parent = unique_name;

		unsigned int pos = parent.find("/" + contents[i]);
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
			directory* pdir = static_cast<directory*> (root_dir.find(parent)->second);
			pdir->addChild(add);

			root_dir.insert(pair<string, node*>(dir, add));

			cout << "adding directory: " << dir << endl;
			// cout << "of parent directory: " << parent <<endl;
			break;
		}
	}
	return ret;
}

//build Ldisk starting with all blocks as one contiguous blocks since they are all free.
bool file_system::file_struct()
{
	bool ret = true;

	cout << "Adding initial blocks, total blocks: " << total_blocks << endl;
	
	blocks* add = new blocks(0, total_blocks-1, true);
	disk_blocks.push_back(add);

	return ret;
}

void file_system::deleteDir(const string& dir_name)
{
	map<string, node*>::iterator iter;

	directory* parent = static_cast<directory*>(current_dir);

	if(parent->getName() == "/")
		iter = root_dir.find(parent->getName() + dir_name);

	else
		iter = root_dir.find(parent->getName() + "/" + dir_name);

	if(iter != root_dir.end())
		root_dir.erase(iter);
}

void file_system::remove_virtual_blocks_range(int start, int end)
{
	deque<blocks*>::iterator iter = disk_blocks.begin();
	
	while(iter != disk_blocks.end())
	{
		blocks* cur = *iter;
	
		if(cur->inLine(start) && cur->inLine(end))
		{
			int old_end = cur->getEnd();
			cur->setEnd(start-1);
			blocks* add = new blocks(start, end, true);
			disk_blocks.push_back(add);
			add = new blocks(end+1, old_end, false);
			disk_blocks.push_back(add);
			join_virtual_blocks();
			break;
		}
		iter++;
	}
}

void file_system::join_virtual_blocks()
{
	sort(disk_blocks.begin(), disk_blocks.end(), &blocks::compare);
	deque<blocks*>::iterator it = disk_blocks.begin();

	for(; it != disk_blocks.end(); it++)
	{
		blocks* cur = *it;
		blocks* next;
		
		if((*it)->blockSz() <= 0)
		{
			deque<blocks*>::iterator i = it;
			delete *i;
			disk_blocks.erase(i);
			
			if(it == disk_blocks.end())
				break;
		}

		if(++it != disk_blocks.end())
		{
			next = *it;
			it--;
		}
		
		else
			break;

		if((cur->isEmpty() && next->isEmpty()) || (!cur->isEmpty() && !next->isEmpty()))
		{
			next->setStart(cur->getStart());
			disk_blocks.erase(it);
			delete cur;
			
			if(++it == disk_blocks.end())
				break;
		}
	}
}

//file would like x amount of space, see if we can add it
bool file_system::handle_file_request(file* myfile, const unsigned int& space_requested)
{
	bool ret(false);

	if(total_used_size >= disk_size)
	{
		cout << "ERROR not enough space available" << endl;
		return ret;
	}

	deque<blocks*>::iterator it = disk_blocks.begin();

	for(; it != disk_blocks.end(); it++)
	{
		blocks* cur = *it;
		
		if(cur->isEmpty())
		{
			int number_blocks = 0;
			int old_size = myfile->getSize();
		
			if(block_size == 1)
				number_blocks = (space_requested)/block_size;
		
			else if((old_size%block_size != 0) && (old_size != 0))
			{
				number_blocks = (space_requested)/block_size;
				total_fragmentation = total_fragmentation - (space_requested%block_size);
			}
		
			else
			{
				number_blocks = (space_requested+1)/block_size;
				total_fragmentation = total_fragmentation + (space_requested%block_size);
			}
		
			cout << "allocating blocks for file: " << myfile->getName() << "- total size: " << space_requested << "- total blocks: " << number_blocks << endl;
			
			int old_start = cur->getStart();
			cur->setStart(old_start+number_blocks);
			int new_end = old_start+(number_blocks-1);
			blocks* add = new blocks(old_start, new_end, false);
			disk_blocks.insert(it, add);
		
			for(int i = old_start; i <= new_end; i++)
				myfile->new_block_address(block_size, i);
		
			ret = true;

			myfile->setSize(old_size + space_requested);
			myfile->setTime();

			total_used_size = total_used_size + space_requested;
			break;
		}
	}

	join_virtual_blocks();
	return ret;
}

const void file_system::printDir(node* root)
{
	directory* parent = dynamic_cast<directory*>(root);
	
	if(parent)
	{
		cout << "Directory: " << parent->getName() << endl;
		vector<node*>* children = parent->getChildren();
		
		for(unsigned int i = 0; i < children->size(); i++)
		{
			node* mynode = dynamic_cast<node*>(children->at(i));
			
			if(!mynode)
				cout << "error cannot cast to node..." << endl;
			
			printDir(mynode);
		}
	}

	else
	{
		file* myfile = dynamic_cast<file*>(root);
		cout << myfile->getName() << endl;
	}
}

file* file_system::fileLookup(const string& unique_name)
{
	directory* parent = static_cast<directory*>(current_dir); 
	vector<node*>* children = parent->getChildren();

	file* ret = NULL;
	
	for(unsigned int i = 0; i < children->size(); i++)
	{
		file* child = dynamic_cast<file*>((*children)[i]);
	
		if(child)
		{
			if(child->getName() == unique_name)
			{
				ret = child;
				break;
			}
		}
	}

	return ret;
}

/******************************* Public functions of the class **********************************/

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
		delete iter->second;

	deque<blocks*>::iterator i_iter;

	for(i_iter = disk_blocks.begin(); i_iter != disk_blocks.end(); ++i_iter)
		delete *i_iter;
}

bool file_system::init_dirs(const string &file_name)
{
	// call the helper function
	file_struct();
	
	bool ret = false;
	string line;
	
	ifstream directory_list(file_name.c_str(), ios::in);
	cout << endl << "opening file: " << file_name << endl << endl;

	//add default root directory
	node* add = new directory("/");
	root_dir.insert(pair<string, node*>("/", add));

	if(directory_list.is_open())
	{
		while(getline(directory_list, line))
		{
			if(line.size() <= 3)
				continue;

			vector<string> contents;
			stringSplit(line, '/', contents);
			line.erase(line.begin());

			// cout << "LINE READ: " << line << endl;

			dir_struct(contents, line);

		}

		ret = true;
	}

	else
		cout << "ERROR: Unable to open file: " << file_name << endl;

	return ret;
}

bool file_system::init_files(const string &file_name)
{
	bool ret = false;
	
	string line;
	
	ifstream file_list(file_name.c_str(), ios::in);
	cout << endl << "opening file: " << file_name << endl << endl;

	if(file_list.is_open())
	{
		while(getline(file_list, line))
		{
			if(line.size() <= 3)
				continue;
			
			size_t p = line.find_first_not_of(" \t");
			line.erase(0, p);

			vector<string> contents;
			string::iterator new_end = unique(line.begin(), line.end(), both_are_spaces);

			//remove unessary spaces, and split line into spaces
			line.erase(new_end, line.end());
			stringSplit(line, ' ', contents);

			// cout << "file read: " << contents[10] << " timestamp: " << contents[9] << "  size:" << contents[6] << endl;

			string file_name = contents[10];
			int pos = file_name.rfind("/");
			file_name.erase(0, pos+1);

			string parent = contents[10];
			parent.erase(pos, string::npos);
			parent.erase(0, 1);

			if(parent.compare("") == 0)
				parent = "/";
			

			if(root_dir.find(parent) != root_dir.end())
			{
				string fsize = contents[6];
				istringstream buffer(fsize);
				
				int size;
				buffer >> size;
				
				directory* pdir = static_cast<directory*>(root_dir.find(parent)->second);
				node* myfile = new file(file_name, 0);
				pdir->addChild(myfile);
				
				file* child = static_cast<file*>(myfile);

				if(!handle_file_request(child, size))
				{
					cout << "error not enough free space to create blocks for file : " << file_name << endl;
					return false;
				}
			}

			else
				cout << "error directory doesn't exist" << endl;
			
			// cout << "file added: " << file_name << endl;
			// cout << "to parent directory " << parent << endl;
		}

		ret = true;
	}

	else
		cout << "ERROR: Unable to open file: " << file_name << endl;
	
	return ret;
}

const void file_system::printDir()
{
	printDir(root_dir.find("/")->second);
}

bool file_system::setCurrentDir(const string & file)
{
	if(root_dir.find(file) != root_dir.end())
	{
		current_dir = root_dir.find(file)->second;
		return true;
	}

	return false;
}

void file_system::ls()
{
	directory* cur = dynamic_cast<directory*>(current_dir);
	cur->listChildren();
}

void file_system::removeFile(const string& child)
{
	directory* cur = dynamic_cast<directory*>(current_dir);
	
	file* pfile = fileLookup(child);

	if(pfile != NULL)
		takeAwayBytes(child, pfile->getSize());

	deleteDir(child);
	cur->removeChild(child);
}


bool file_system::newDir(const string &add, const string &unique)
{
	bool ret = false;
	
	if(root_dir.find(unique) == root_dir.end())
	{

		directory* cur = dynamic_cast<directory*>(current_dir);
		
		if(cur->hasChild(add))
		{
			cout << "cannot add directory, file or directory with the same name exists..." << endl;
			return ret;
		}
		
		ret = true;
		node* new_dir;

		if(cur->getName() == "/")
			new_dir = new directory("/" + add);
		
		else
			new_dir = new directory("/" + add);
		
		cur->addChild(new_dir);
		cout << "adding directory " << unique << endl;
		root_dir.insert(pair<string, node*>(unique, new_dir));
	}

	else
		cout << "cannot add directory, file or directory with the same name exists..." << endl;

	return ret;
}

bool file_system::newFile(const string &add, const string &unique)
{
	bool ret = false;

	directory* cur = dynamic_cast<directory*>(current_dir);
	
	if(!cur->hasChild(add))
	{
		node* new_file = new file(add, 0);
		cur->addChild(new_file);
		cout << "adding file: " << add << endl;
		ret = true;
		root_dir.insert(pair<string, node*>(unique, new_file));
	}

	else
		cout << "file already exists.." << endl;
	
	return ret;
}

const void file_system::bfsFileInfo()
{
	queue<node*> bfs;
	node* current(root_dir.find("/")->second);
	bfs.push(current);

	while(!bfs.empty())
	{
		current = bfs.front();
		directory* parent = dynamic_cast<directory*>(current);
	
		if(parent)
		{
			vector<node*>* children = parent->getChildren();
	
			for(unsigned int i = 0; i < children->size(); i++)
			{
				node* mynode = dynamic_cast<node*>(children->at(i));
	
				if(!mynode)
				{
					cout << "error cannot cast to node..." << endl;
					return;
				}

				bfs.push(mynode);
			}
		}
	
		else
		{
			file* myfile = dynamic_cast<file*>(current);
			myfile->prfiles();
		}

		bfs.pop();
	}
}

const void file_system::bfsTraverse()
{
	queue<node*> bfs;
	node* current(root_dir.find("/")->second);
	bfs.push(current);

	while(!bfs.empty())
	{
		current = bfs.front();
		directory* parent = dynamic_cast<directory*>(current);
	
		if(parent)
		{
			cout << "Directory: " << parent->getName() << endl;
			vector<node*>* children = parent->getChildren();
	
			for(unsigned int i = 0; i < children->size(); i++)
			{
				node* mynode = dynamic_cast<node*>(children->at(i));
	
				if(!mynode)
				{
					cout << "error cannot cast to node..." << endl;
					return;
				}
	
				bfs.push(mynode);
			}
		}

		else
		{
			file* myfile = dynamic_cast<file*>(current);
			cout << "File: " << myfile->getName() << endl;
		}
		
		bfs.pop();
	}
}

const void file_system::printBlocks()
{
	join_virtual_blocks();
	cout << endl << "LDISK structure..." << endl;
	deque<blocks*>::iterator iter;
	iter = disk_blocks.begin();

	for(; iter != disk_blocks.end(); iter++)
	{
		if((*iter)->blockSz() <= 0)
		{
			deque<blocks*>::iterator it = iter;
			delete *it;
			disk_blocks.erase(it);

			if(iter == disk_blocks.end())
				break;

			else
				continue;
		}

		string fr = "Full";

		if((*iter)->isEmpty())
			fr = "Free";

		cout << "node: -with: " << (*iter)->blockSz() << " contiguous " << fr << " blocks, range: " << (*iter)->getStart() << ":" << (*iter)->getEnd() << endl;
	}
}

const void file_system::printDiskInfo()
{
	cout << "Total disk size: " <<  disk_size << endl;
	cout << "Total blocks: " << total_blocks << endl;
	cout << "Total used space (not including fragmentation): " << total_used_size  << endl;
	cout << "Fragmentation: " << total_fragmentation << endl;
	printBlocks();
}

void file_system::takeAwayBytes(const string &unique, const unsigned int& bytes)
{
	file* child(fileLookup(unique));

	if(child == NULL)
	{
		cout << "ERROR file does not exist" << endl;
		return;
	}

	if(child->getSize() < bytes)
	{
		cout << "can't delete more than file size " << endl;
		return;
	}

	int old_size = child->getSize();
	int block = 0;
	
	if(block_size == 1)
		block = (bytes)/block_size;
	
	else if((old_size%block_size != 0) && (old_size != 0))
	{
		block = (bytes+1)/block_size;
		total_fragmentation = total_fragmentation - (bytes%block_size);
	}
	
	else
	{
		block = (int)floor((bytes)/block_size);
		total_fragmentation = total_fragmentation + (bytes%block_size);
	}
	
	child->setSize(child->getSize() - bytes);
	child->setTime();

	cout << "removing: " << bytes << " bytes, and " << block << " blocks from " << child->getName() << endl;

	vector<int> blocks = child->getNBytes(block);
	
	for(unsigned int i = 0; i < blocks.size(); i ++)
	{
		int end = blocks[i];
		int start = blocks[++i];
		remove_virtual_blocks_range(start, end);
	}
	
	total_used_size = total_used_size-bytes;
}

void file_system::giveBytes(const string &unique, const unsigned int& bytes)
{
	file* child(fileLookup(unique));
	
	if(child == NULL)
	{
		cout << "ERROR file does not exist" << endl;
		return;
	}
	
	if((disk_size - total_used_size - total_fragmentation) < bytes)
	{
		cout << "ERROR can't append more bytes than disk size.." << endl;
		return;
	}
	
	else
		handle_file_request(child, bytes);	
}
