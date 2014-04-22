#include "disk_block.h"

disk_block::disk_block(const unsigned int blockAddress, const unsigned int blockSize, const unsigned int blockID):
	block_address(blockAddress),
	block_size(blockSize),
	block_id(blockID)
{

}

disk_block::~disk_block()
{
	
}

const int disk_block::getBlockAddress()
{
	return block_address;
}

void disk_block::setBlockAddress(int bAddress)
{
	block_address = bAddress;
}

const int disk_block::getBlockSize()
{
	return block_size;
}

void disk_block::setBlockSize(int bSize)
{
	block_size = bSize;
}

const int disk_block::getBlockID()
{
	return block_id;
}

void disk_block::setBlockID(int bID)
{
	block_id = bID;
}