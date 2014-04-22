#include "virtual_block.h"

virtual_block::virtual_block(const bool& free):
	free(free)
	{

	}

virtual_block::~virtual_block()
{
	map<int, disk_block*>::iterator iter;
	iter = Map->begin();

	for(; iter != Map->end(); iter++)
	{
		delete iter->second;
		delete Map;
	}
}

const bool virtual_block::isFree()
{
	return free;
}

map<int, disk_block*>* virtual_block::getMap()
{
	return Map;
}

void virtual_block::replaceList(map<int, disk_block*>* new_map)
{
	Map = new_map;
}