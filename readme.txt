Suketu Shah
CS-492-A
Assignment 3: File System Implementation

In order to compile the project: 

Unzip and run the command make

In order to run it:
./filesystem -f inputs/dir_list.txt inputs/file_list.txt -s 524288 -b 2

Executable name: ./filesystem
Minimum disk size needed to run my input: 524288

Input files are located inside inside the folder named: inputs

In order to generate a new directory list file: find ./ -type d > dir_list.txt

In order to generate a new file list file: find ./ -type f > file_list.txt

and move them inside the inputs folder.

Supported commands:
	cd
	cd ..
	ls
	mkdir
	create
	append
	remove
	delete
	exit
	dir
	prfiles
	prdisk

All the documentation has been provided in each of the header files.