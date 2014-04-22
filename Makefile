#Suketu Shah
#Moodle Login Name: sshah75 
#CS-492-A
#Homework 3 - File System Implementation

NAME = filesystem
SRC = directory.cpp disk_block.cpp file.cpp file_system.cpp main.cpp node.cpp virtual_block.cpp
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