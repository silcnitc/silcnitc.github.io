#include "fileSystem.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>
#include "inode.h"
#include "virtualDisk.h"

extern BLOCK* disk;

/*
	This function returns an empty fat entry if present.
	NOTE: The return address will be the relative word address corresponding to the filename entry in the basic block.
*/
int FindEmptyInodeEntry(){
	int i,j,entryFound = 0,entryNumber = 0;
	for(j = INODE ; j < INODE + NO_OF_INODE_BLOCKS ; j++){
		for(i = INODE_ENTRY_FILENAME; i < BLOCK_SIZE ; i = i + INODE_ENTRY_SIZE){
			if( getValue(disk[j].word[i]) == -1 ){
				entryNumber = (((j - INODE) * BLOCK_SIZE) + i);
				entryFound = 1;
				break;
			}
		}
		if(entryFound == 1)
			break;
	}
	if( entryNumber > INODE_SIZE ){
		printf("INODE is full.\n");
		return -1;
	}
	return (entryNumber-INODE_ENTRY_FILENAME);
}


/*
	This function adds the type, name and size of the file to corresponding entry in the rootfile.
*/
void AddEntryToMemRootFile(int startIndexInRootFile, int fileType, char *nameOfFile, int size_of_file)
{
	int baseAddress=ROOTFILE * BLOCK_SIZE + startIndexInRootFile;
	storeStringValueAt( baseAddress + ROOTFILE_ENTRY_FILENAME, nameOfFile);
	storeValueAt( baseAddress + ROOTFILE_ENTRY_FILESIZE , size_of_file );
	storeValueAt( baseAddress + ROOTFILE_ENTRY_FILETYPE, fileType );
}

/*
	This function adds the name, size and data block addresses of the file to corresponding entry in the inode.
	The first arguement is a relative address
	An entry is also added in the root file
*/
void AddEntryToMemInode(int startIndexInInode, int fileType, char *nameOfFile, int size_of_file, int* addrOfDataBlocks){

	int i,baseAddress=INODE * BLOCK_SIZE + startIndexInInode;
	storeValueAt( baseAddress + INODE_ENTRY_FILETYPE, fileType );
	storeStringValueAt( baseAddress + INODE_ENTRY_FILENAME, nameOfFile);
	storeValueAt( baseAddress + INODE_ENTRY_FILESIZE , size_of_file );
	for(i=0;i<INODE_NUM_DATA_BLOCKS;i++)
		storeValueAt( baseAddress + INODE_ENTRY_DATABLOCK + i , addrOfDataBlocks[i] );
	
	AddEntryToMemRootFile(startIndexInInode/INODE_ENTRY_SIZE*ROOTFILE_ENTRY_SIZE, fileType, nameOfFile, size_of_file);
}


/*
	This function removes the Root file entry corresponding to the first arguement.
	NOTE: locationOfRootFile - relative word address of the name field in the RootFile.
	This is done as follows:
	1. The name field is set to empty string.
	2. filetype is set to -1.
	3. filesize is set to 0.
	The memory copy is not committed.
*/
int removeRootFileEntry(int locationOfRootFile){

	int i;
	int blockNumber = ROOTFILE + locationOfRootFile / BLOCK_SIZE;
	int startWordNumber = locationOfRootFile % BLOCK_SIZE;

	storeValue(disk[blockNumber].word[startWordNumber + ROOTFILE_ENTRY_FILETYPE], -1);
	storeValue(disk[blockNumber].word[startWordNumber + ROOTFILE_ENTRY_FILENAME], -1);
	storeValue(disk[blockNumber].word[startWordNumber + ROOTFILE_ENTRY_FILESIZE], 0);
	
	return 0;
}

/*
	This function removes the fat entry corresponding to the first arguement.
	NOTE: locationOfInode - relative word address of the name field in the fat.
	This is done as follows:
	1. The name field is set to empty string.
	2. The data block entries is set to -1.
	3. filetype set to -1
	4. filesize set to 0
	Corresponding root file entry is also removed.
	The memory copy is not committed.
*/
int removeInodeEntry(int locationOfInode){
	int i;
	int blockNumber = INODE + locationOfInode / BLOCK_SIZE;
	int startWordNumber = locationOfInode % BLOCK_SIZE;

	storeValue(disk[blockNumber].word[startWordNumber + INODE_ENTRY_FILETYPE], -1);
	storeValue(disk[blockNumber].word[startWordNumber + INODE_ENTRY_FILENAME], -1);
	storeValue(disk[blockNumber].word[startWordNumber + INODE_ENTRY_FILESIZE], 0);
	for(i=0;i<INODE_NUM_DATA_BLOCKS;i++)
		storeValue(disk[blockNumber].word[startWordNumber + INODE_ENTRY_DATABLOCK + i], -1);
	
	removeRootFileEntry(locationOfInode / INODE_ENTRY_SIZE * ROOTFILE_ENTRY_SIZE);
	
	return 0;
}

/*
	This function checks if a file having name as the first arguement is present on the disk file.
	This is done as follows:
	1. It checks the entry in the fat block. 
		a.If a file with same name exists then the function returns the relative word
		address of the entry having the same name.
		b. returns -1 if the file name doesnt exist
*/
int getInodeEntry(char *name){
	int i,j;
	if(name==NULL)
		return -1;
	for(j = INODE ; j < INODE + NO_OF_INODE_BLOCKS ; j++)
	{
		for(i = INODE_ENTRY_FILENAME ; i < BLOCK_SIZE ; i = i + INODE_ENTRY_SIZE)
		{
			if(strcmp(disk[j].word[i], name) == 0 && getValue(disk[j].word[i]) != -1)		
				return (((j - INODE) * BLOCK_SIZE) + i - INODE_ENTRY_FILENAME);
		}
	}
	return -1;
}


/*
	This function returns the data block entries(pass by pointer) corresponding to the address specified by the locationOfInode.
	Third argument specifies the type of file (assembly code or data file)
	NOTE: locationOfInode - relative word address of the name field in the fat.
*/
int getDataBlocks(int *dataBlockAddr, int locationOfInode)
{
	int i,a;
	for(i=0;i<INODE_NUM_DATA_BLOCKS;i++)
		dataBlockAddr[i] = getValue(disk[INODE + locationOfInode / BLOCK_SIZE].word[locationOfInode % BLOCK_SIZE + INODE_ENTRY_DATABLOCK + i]);
	return 0;
}
