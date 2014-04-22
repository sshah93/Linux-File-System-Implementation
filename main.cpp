#include "header.h"
#include "node.h"
#include "file_system.h"

bool handleInput(string& input);

// the only global variables
// pointer to the new file system that will be created using the files and dirs
file_system* new_file_system;
// stores the current dir
string current_dir;

int main(int argc, char* argv[])
{
	if(argc != 8)
	{
		// cout << "Not enough arguments" << endl;
		cout << "Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	}

	if(argv[1] != "-f")
	{
		// cout << "You drunk? :P" << endl;
		cout << "Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	}

	if(argv[4] != "-s")
	{
		// cout << "You drunk? :P" << endl;
		cout << "Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	}

	if(argv[6] != "-b")
	{
		// cout << "You drunk? :P" << endl;
		cout << "Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	}

	string dir_list = argv[2];
	
	string file_list = argv[3];

	unsigned int disk_size = atoi(argv[5]);

	if(!disk_size)
	{
		cout << "Invalid disk size. It has to be a number greater than 0" << endl;
		return -1;
	}

	unsigned int block_size = atoi(argv[7]);

	if(!block_size)
	{
		cout << "Invalid block size. It has to be a number greater than 0" << endl;
		return -1;
	}

	// local variables
	bool res;
	string line;

	new_file_system = new file_system(disk_size, block_size);
	
	res = new_file_system->initialize_directories(dir_list);

	if(!res)
	{
		cout << "Well your dirs weren't initialize so too bad!" << endl;
		return -1;
	}	

	res = new_file_system->initialize_files(file_list);

	if(!res)
	{
		cout << "Wow your files didn't initialize either" << endl;
		return -1;
	}

	cout << "Finished preloading files and directories " << endl;

	new_file_system->setCurrentDir("/");
	current_dir = "/";

	cout << "Total dirs: " << endl;
	new_file_system->print_dir();

	while(1)
	{
		cout << "cs492@suketu: " << current_dir;
		cout << "$ ";
		getline(cin, line);
		res = handleInput(line); 

		if(!res)
		{
			break;
		}

		cin.clear();
		cout << endl;
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
			cout << "Incorrect input by the user!" << endl;
			return -1;
			//continue;
		}

		else if(contents[1].find("..") == 0)
		{
			if(current_dir.compare("/") == 0)
			{
				cout << "Unable to change directory" << endl;
				return -1;
				//continue;
			}

			current_dir = current_dir.substr(0, current_dir.size()-1);
			
			int pos = current_dir.rfind("/");
			current_dir.erase(pos+1, string::npos);
			
			dir = current_dir;
			
			if(dir.compare("/") != 0)
			{
				dir = dir.substr(0, dir.size()-1);
			}
		}

		else
		{
			dir = current_dir + contents[1];
		}

		if(!new_file_system->setCurrentDir(dir))
		{
			cout << "Unable to change directory" << endl;
		}	

		else
		{
			if(dir.compare("/") != 0)
			{
				current_dir = dir + "/";
			}
		}
	}
	
	// ls case
	else if(!input.find("ls"))
	{
		new_file_system->list();
	}
	
	else if(!input.find("mkdir"))
	{
		vector<string> contents;
		stringSplit(input, ' ', contents);

		string unique = current_dir + contents[1];
		new_file_system->add_dir_under_current(contents[1], unique);
	}

	else if(!input.find("create"))
	{
		vector<string> contents;
		stringSplit(input, ' ', contents);

		string unique = current_dir + contents[1];
		new_file_system->add_file_under_current(contents[1], unique);
	}
	
	else if(!input.find("append"))
	{
		
	}
	
	else if(!input.find("remove"))
	{

	}
	
	else if(!input.find("delete"))
	{
		
	}
	
	else if(!input.find("exit"))
	{
		cout << endl << "exit" << endl;
		return false; 
	}

	else if(!input.find("dir"))
	{
		new_file_system->bfs_traverse();
	}
	
	else if(!input.find("prfiles"))
	{
		new_file_system->bfs_file_info();
	}
	
	else if(!input.find("prdisk"))
	{
		new_file_system->print_disk_info();	
	}
	
	else 
	{
		cout << "Error: Unrecognized command" << endl;
	}

	return true;
}