/*
Suketu Shah
CS-492-A
Assignment 3: File System Implementation
*/

#include "node.h"
#include "file_system.h"

bool handleInput(string& input);

// the only global variables
// pointer to the new file system that will be created using the files and dirs
file_system* new_file_system;
// storesult the current dir
string current_dir;

int main(int argc, char* argv[])
{
	if(argc != 8)
	{
		// cout << "Not enough arguments" << endl;
		cout << "1. Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	}

	string arg1 = argv[1];

	if(arg1.compare("-f") != 0)
	{
		// cout << "You drunk? :P" << endl;
		cout << "2. Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	}

	string arg4 = argv[4];

	if(arg4.compare("-s") != 0)
	{
		// cout << "You drunk? :P" << endl;
		cout << "3. Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	}

	string arg6 = argv[6];

	if(arg6.compare("-b") != 0)
	{
		// cout << "You drunk? :P" << endl;
		cout << "4. Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	} 

	string dir_list = argv[2];
	string file_list = argv[3];
	unsigned int disk_size = atoi(argv[5]);
	unsigned int block_size = atoi(argv[7]);

	if(!disk_size)
	{
		cout << "Invalid disk size. It has to be a number greater than 0" << endl;
		return -1;
	}

	if(!block_size)
	{
		cout << "Invalid block size. It has to be a number greater than 0" << endl;
		return -1;
	}

	// local variables
	bool result;
	string line;

	new_file_system = new file_system(disk_size, block_size);
	result = new_file_system->init_dirs(dir_list);

	if(!result)
	{
		cout << "Unable to initialize the directories. Something went wrong!" << endl;
		return -1;
	}	

	result = new_file_system->init_files(file_list);

	if(!result)
	{
		cout << "Unable to initialize your files, might want to give increase disk size" << endl;
		return -1;
	}

	cout << endl << "Done loading files and directories " << endl << endl;

	new_file_system->setCurrentDir("/");
	current_dir = "/";

	// cout << "Total dirs: " << endl;
	// new_file_system->print_directory();

	cout << "************************************************************************************************************" << endl;

	cout << endl << "Here's your minishell: " << endl << endl;

	while(1)
	{
		cout << "cs492@suketu:" << current_dir;
		cout << "$ ";
		
		getline(cin, line);
		result = handleInput(line); 

		if(!result)
		{
			break;
		}

		cin.clear();
		//cout << endl;
	}

	return 0;
}

bool handleInput(string& input)
{
	// cd case
	if(!input.find("cd"))
	{
		vector<string> contents;

		stringSplit(input, ' ', contents);

		string dir;

		if(contents.size() != 2)
		{
			cout << "cd only takes 1 argument" << endl;
			return -1;
			//continue;
		}

		else if(contents[1].find("..") == 0)
		{
			if(current_dir.compare("/") == 0)
			{
				cout << "Can't change directory" << endl;
				return -1;
				//continue;
			}

			current_dir = current_dir.substr(0, current_dir.size()-1);

			int pos = current_dir.rfind("/");
			current_dir.erase(pos+1, string::npos);

			dir = current_dir;

			if(dir.compare("/") != 0)
				dir = dir.substr(0, dir.size()-1);
		}

		else
			dir = current_dir + contents[1];

		if(!new_file_system->setCurrentDir(dir))
			cout << "Unable to change directory" << endl;

		else
		{
			if(dir.compare("/") != 0)
				current_dir = dir + "/";
		}
	}

	// ls case
	else if(!input.find("ls"))
	{
		new_file_system->ls();
	}

	// mkdir case
	else if(!input.find("mkdir"))
	{
		vector<string> contents;
		stringSplit(input, ' ', contents);

		if(contents.size() < 2)
		{
			cout << "Not enough arguments" << endl;
		}

		else if(contents.size() > 2)
		{
			cout << "More then enough arguments" << endl;
		}

		else
		{
			string unique = current_dir + contents[1];
			new_file_system->newDir(contents[1], unique);
		}
	}

	// create case
	else if(!input.find("create"))
	{
		vector<string> contents;
		stringSplit(input, ' ', contents);

		if(contents.size() < 2)
		{
			cout << "Not enough arguments" << endl;
		}

		else if(contents.size() > 2)
		{
			cout << "More then enough arguments" << endl;
		}

		else
		{
			string unique = current_dir + contents[1];
			new_file_system->newFile(contents[1], unique);
		}
	}

	// append case
	else if(!input.find("append"))
	{
		vector<string> contents;
		stringSplit(input, ' ', contents);

		if(contents.size() < 3)
		{
			cout << "Not enough arguments" << endl;
		}

		else if(contents.size() > 3)
		{
			cout << "More then enough arguments" << endl;
		}

		else
		{
			string unique = contents[1];
			istringstream buffer(contents[2]);

			int size;
			buffer >> size;

			new_file_system->giveBytes(unique, size);
		}
	}

	// remove case
	else if(!input.find("remove"))
	{
		vector<string> contents;
		stringSplit(input, ' ', contents);

		if(contents.size() < 3)
		{
			cout << "Not enough arguments" << endl;
		}

		else if(contents.size() > 3)
		{
			cout << "More then enough arguments" << endl;
		}

		else
		{
			string unique = contents[1];
			istringstream buffer(contents[2]);

			int size;
			buffer >> size;

			new_file_system->takeAwayBytes(unique, size);
		}
	}

	// delete case
	else if(!input.find("delete"))
	{
		vector<string> contents;
		stringSplit(input, ' ', contents);

		if(contents.size() < 2)
		{
			cout << "Not enough arguments" << endl;
		}

		else if(contents.size() > 2)
		{
			cout << "More then enough arguments" << endl;
		}

		else
		{
			string unique = contents[1];
			new_file_system->removeFile(unique);
		}
	}

	// exit case
	else if(!input.find("exit"))
	{
		cout << "exit" << endl;
		return false; 
	}
	// dir case
	else if(!input.find("dir"))
	{
		new_file_system->bfsTraverse();
	}

	// prfiles case
	else if(!input.find("prfiles"))
	{
		new_file_system->bfsFileInfo();
	}

	// prdisk case
	else if(!input.find("prdisk"))
	{
		new_file_system->printDiskInfo();	
	}

	// unknown command
	else 
	{
		cout << "Error: Unrecognized command" << endl;
	}

	return true;
}