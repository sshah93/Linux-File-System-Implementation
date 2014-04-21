#include "node.h"

int main(int argc, char* argv[])
{
	if(argc != 8)
	{
		cout << "Not enough arguments" << endl;
		cout << "Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	}

	if(argv[1] != "-f")
	{
		cout << "You drunk? :P" << endl;
		cout << "Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	}

	if(argv[4] != "-s")
	{
		cout << "You drunk? :P" << endl;
		cout << "Usage: <./filesystem> -f dir_list.txt file_list.txt -s <disk_size> -b <block_size>" << endl;
		return -1;
	}

	if(argv[6] != "-b")
	{
		cout << "You drunk? :P" << endl;
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

		
}