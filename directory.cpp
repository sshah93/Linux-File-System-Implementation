/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#include "directory.h"
#include "file.h"

directory::directory(const string& name):node(), dir_name(name)
{

}

directory::~directory()
{

}

void directory::addChild(node* child)
{
	children_node.push_back(child);
}

void directory::removeChild(const string& name)
{
	bool ret = false;

	vector<node*>::iterator iter = children_node.begin();
	string dir = "";

	if(dir_name == "/")
	{
		dir = "/";
	}

	for(; iter != children_node.end(); iter++)
	{
		directory* parent = dynamic_cast<directory*>(*iter);

		if(parent)
		{
			if((parent->getName().find("/" + name) != std::string::npos) 
					|| (parent->getName().find(name) != std::string::npos))
			{
				if(parent->getChildrenSize() > 0)
					cout << "Directory is not empty" << endl;
				
				else
				{
					children_node.erase(iter);
					ret = true;
					cout << "Directory: " << parent->getName() << " has been removed" << endl;
				}
				
				break;
			}
		}

		else
		{
			file* child = dynamic_cast<file*>(*iter);

			if(child)
			{
				if(child->getName() == name)
				{
					children_node.erase(iter);
					cout << "File: "<< child->getName() << " has been removed" << endl;
					break;
				}
			}
		}
	}
}

const string directory::getName()
{
	return dir_name;
}

std::vector<node*>* directory::getChildren()
{
	return &children_node;
}

const int directory::getChildrenSize()
{
	return children_node.size();
}

void directory::listChildren()
{
	unsigned int i;
	directory* parent_dir;
	file* child_dir;

	for(i = 0; i < children_node.size(); i++)
	{
		parent_dir = dynamic_cast<directory*>(children_node[i]);

		if(parent_dir)
		{
			vector<string> contents;
			stringSplit(parent_dir->getName(), '/', contents);
			cout << contents[contents.size() - 1] << endl;
		}

		else
		{
			child_dir = dynamic_cast<file*>(children_node[i]);
			string time = child_dir->getTime();
			cout << child_dir->getName() << setw(50- child_dir->getName().size()) << " size: " << child_dir->getSize() <<  " bytes" << "\t" << setw(25-time.size()) << time << endl;
		}
	}
}

const bool directory::hasChild(const string& name)
{
	bool ret = false;

	for(unsigned int i = 0; i < children_node.size(); i++)
	{
		directory* parent = dynamic_cast<directory*>(children_node[i]);

		if(parent)
		{
			if((parent->getName() == name) || (parent->getName() == "/" + name))
			{
				ret = true;
				break;
			}
		}

		else
		{
			file* child = dynamic_cast<file*>(children_node[i]);

			if(child->getName() == name)
			{
				ret = true;
				break;
			}
		}
	}
	return ret;
}