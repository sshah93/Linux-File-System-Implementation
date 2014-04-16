#Suketu Shah
#Moodle Login Name: sshah75 
#CS-492-A
#Homework 3 - File System Implementation

NAME = filesystem
SRC = node.cpp directory.cpp file.cpp disk_block.cpp block_addresses.cpp file_system.cpp main.cpp
OBJ = $(SRC:.c=.o)
RM = rm -f
LIBS = -ggdb
CCFLAGS = -std=c++0x

all: $(OBJ) 
	g++ $(SRC) $(LIBS) -o $(NAME) $(CCFLAGS)

clean:
	-$(RM) *.o
	-$(RM) *~
	-$(RM) \#*
	-$(RM) *.core

fclean: clean
	-$(RM) $(NAME)
	
re: fclean all