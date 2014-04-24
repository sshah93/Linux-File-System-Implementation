#include "directory.h"
#include "file.h"
#include "node.h"

directory::directory(const string& name):node(), dir_name(name)
{

}

directory::~directory()
{

}

void directory::add_child(node* child)
{
	children_node.push_back(child);
}

void directory::remove_child(const string& name)
{
	bool ret = false;
	
	vector<node*>::iterator iter = children_node.begin();
	string dir = "";
	
	if(dir_name == "/")
	{
		dir = "/";
	}
	
	for(; iter != children_node.end(); i++)
	{
		directory* parent = dynamic_cast<directory*>(*iter);
		
		if(parent)
		{
			if(parent->getName() == ("/" + name))
			{
				if(parent->getChildrenSize() > 0)
					cout << "You're trying to delete a non-empty directory" << endl;
					
				else
				{
					children_node.erase(iter);
					ret = true;
					cout << "directory: " << parent->getName() << " removed ..." << endl;
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
					cout << "File: "<< child->getName() << " removed ..." << endl;
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
	int i;
	directory* parent_dir;
	file* child_dir;

	for(i = 0; i < children_node.size(); i++)
	{
		parent_dir = dynamic_cast<directory*>(children_node[i]);

		if(parent_dir)
		{
			std::vector<string> contents;
			stringSplit(parent_dir->getName(), '/', contents);
			cout << contents[contents.size() - 1] << endl;
		}

		else
		{
			child_dir = dynamic_cast<file*>(children_node[i]);
			cout << child_dir->getName() << setw(50- child_dir->getName().size()) << " size: " << child_dir->getSize() <<  " bytes" << endl;
		}
	}
}

const bool directory::hasChild(const string& name)
{
	bool ret = false;
	
	for(i = 0; i < children_node.size(); i++)
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
