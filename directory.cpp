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

const string directory::getName()
{
	return dir_name;
}

std::vector<node*>* directory::getChildren()
{
	return &children_node;
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