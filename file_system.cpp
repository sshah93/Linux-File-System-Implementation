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
	{
		map<string, node*>::iterator iter;
		for(iter = root_dir.begin(); iter != root_dir.end(); ++iter)
		{
			delete iter->second;
		}
	}
	{
		deque<virtual_block*>::iterator iter;
		for(iter = disk_blocks.begin(); iter != disk_blocks.end(); ++iter)
		{
			delete *iter;
		}
	}
}

const void file_system::print_blocks()
{
	merge();
	cout << "LDISK structure..." << endl;
	deque<virtual_block*>::iterator iter;
	iter = disk_blocks.begin();

	for(; iter != disk_blocks.end() ; iter++)
	{
		if((*iter)->getSize() <= 0)
		{
			deque<virtual_block*>::iterator it = iter;
			delete *it;
			disk_blocks.erase(it);
			if(iter == disk_blocks.end())
			{
				break;
			}
			else
			{
				continue;
			}
		}
		string fr("FULL");
		if((*iter)->isFree())
		{
			fr = "FREE";
		}
		cout << "node: -with: " << (*iter)->getSize() << " contiguous " << fr << " blocks, range: " << (*iter)->getStart() << ":" << (*iter)->getEnd() << endl;
	}
}

file* file_system::find_file(const string& unique_name)
{
	directory* pdir = static_cast<directory*>(current_dir); 
	vector<node*>* children = pdir->getChildren();

	file* ret = NULL;
	for(unsigned int i = 0; i < children->size(); i++)
	{
		file* pfile = dynamic_cast<file*>((*children)[i]);
		if(pfile)
		{
			if(pfile->getName() == unique_name)
			{
				ret = pfile;
				break;
			}
		}
	}
	return ret;
}

void file_system::delete_range(int start, int end)
{
	deque<virtual_block*>::iterator iter = disk_blocks.begin();
	while(iter != disk_blocks.end())
	{
		virtual_block* cur = *iter;
		if(cur->inRange(start) && cur->inRange(end))
		{
			int old_end = cur->getEnd();
			cur->setEnd(start-1);
			virtual_block* add = new virtual_block(start, end, true);
			disk_blocks.push_back(add);
			add = new virtual_block(end+1, old_end, false);
			disk_blocks.push_back(add);
			merge();
			break;
		}
		iter++;
	}
}

void file_system::remove_bytes_from_file(const string &unique, const unsigned int& bytes)
{
	file* pfile(find_file(unique));
	if(pfile == NULL)
	{
		cout << "ERROR file does not exist" << endl;
		return;
	}

	if(pfile->getSize() < bytes)
	{
		cout << "can't delete more than file size " << endl;
		return;
	}
	pfile->setSize(pfile->getSize() - bytes);

	int block = (bytes+1)/block_size;
	cout << "removing: " << bytes << " bytes, and " << block << " blocks from " << pfile->getName() << endl;

	vector<int> blocks = pfile->get_last_n(block);
	for(unsigned int i = 0; i < blocks.size(); i ++)
	{
		int end = blocks[i];
		int start = blocks[++i];
		cout << start << ":" << end << endl;
		delete_range(start, end);
	}
	total_used_size = total_used_size-bytes;
}

void file_system::add_bytes_to_file(const string &unique, const unsigned int& bytes)
{
	file* pfile(find_file(unique));
	if(pfile == NULL)
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
	{
		pfile->setSize(pfile->getSize() + bytes);

		handle_file_request(pfile, bytes);
	}

}

void file_system::merge()
{
	sort(disk_blocks.begin(), disk_blocks.end(), &virtual_block::compare);
	deque<virtual_block*>::iterator it = disk_blocks.begin();

	for(; it != disk_blocks.end(); it++)
	{
		virtual_block* cur = *it;
		virtual_block* next;
		if((*it)->getSize() <= 0)
		{
			deque<virtual_block*>::iterator i = it;
			delete *i;
			disk_blocks.erase(i);
			if(it == disk_blocks.end())
			{
				break;
			}
		}
		if(++it != disk_blocks.end())
		{
			next = *it;
			it--;
		}
		else
		{
			break;
		}

		if((cur->isFree() && next->isFree()) || (!cur->isFree() && !next->isFree()))
		{
			next->setStart(cur->getStart());
			disk_blocks.erase(it);
			delete cur;
			if(++it == disk_blocks.end())
			{
				break;
			}
		}
	}
}

//file would like x amount of space, see if we can add it
bool file_system::handle_file_request(file* file, const unsigned int& space_requested)
{
	bool ret(false);

	if(total_used_size >= disk_size)
	{
		cout << "ERROR not enough space available" << endl;
		return ret;
	}
	deque<virtual_block*>::iterator it = disk_blocks.begin();

	for(; it != disk_blocks.end(); it++)
	{
		virtual_block* cur = *it;
		if(cur->isFree())
		{
			int blocks = 0;
			if(block_size == 1)
			{
				blocks = (space_requested)/block_size;
			}
			else
			{
				blocks = (space_requested+1)/block_size;
			}
			int old_start = cur->getStart();
			cur->setStart(old_start+blocks);
			int new_end = old_start+(blocks-1);
			virtual_block* add = new virtual_block(old_start, new_end, false);
			disk_blocks.insert(it, add);
			ret = true;

#ifdef DEBUG
			cout << "allocating blocks for file " << file->getName() << " total size: " << space_requested << ", total blocks: " << blocks << endl;
#endif
			total_used_size = total_used_size + space_requested;
			total_fragmentation = total_fragmentation + (space_requested%block_size);
			for(int i = old_start; i <= new_end; i++)
			{
				file->add_address(block_size, i);
			}
			break;
		}
	}
	merge();
	return ret;
}

//build Ldisk starting with all blocks as one contiguous virtual_block since they are all free.
bool file_system::build_file_structure()
{
	bool ret(true);

	cout << "Adding initial blocks, total blocks: " << total_blocks << endl;
	virtual_block* add = new virtual_block(0, total_blocks-1, true);
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
			stringSplit(line, '/', contents);
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
			if(line.size() <= 3)
			{
				continue;
			}
			vector<string> contents;
			string::iterator new_end = unique(line.begin(), line.end(), both_are_spaces);

			//remove unessary spaces, and split line into spaces
			line.erase(new_end, line.end());
			stringSplit(line, ' ', contents);
			
			cout << "file read: " << contents[10] << " timestamp: " << contents[9] << "  size:" << contents[6] << endl;

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
				node* myfile = new file(file_name, size);
				pdir->add_child(myfile);
				file* pfile = static_cast<file*>(myfile);
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

			cout << "file added: " << file_name << endl;
			cout << "to parent directory " << parent << endl;
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
			vector<node*>* children = pdir->getChildren();
			
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
			myfile->printInfo();
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
			vector<node*>* children = pdir->getChildren();
		
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

const void file_system::print_directory(node* root)
{
	directory* pdir = dynamic_cast<directory*>(root);
	
	if(pdir)
	{
		cout << "Directory: " << pdir->getName() << endl;
		vector<node*>* children = pdir->getChildren();
		for(unsigned int i = 0; i < children->size(); i++)
		{
			node* mynode = dynamic_cast<node*>(children->at(i));
			if(!mynode)
			{
				cout << "error cannot cast to node..." << endl;
			}
			print_directory(mynode);
		}
	}
	else
	{
		file* myfile = dynamic_cast<file*>(root);
		cout << myfile->getName() << endl;
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
	cur->listChildren();
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
	cout << "total disk size: " <<  disk_size << " -total blocks: " << total_blocks << "- total used space (not including fragmentation): " << 
		total_used_size  << " - fragmentation: " << total_fragmentation << endl;
	print_blocks();
}