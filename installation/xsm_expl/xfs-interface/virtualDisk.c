#include "virtualDisk.h"
#include <stdlib.h>
#include <stdio.h>
#include "disk.h"

extern BLOCK* disk;

void _disk_init()
{
	disk=(BLOCK*)malloc((NO_BLOCKS_TO_COPY + EXTRA_BLOCKS)*sizeof(BLOCK));			
	// disk contains the memory copy of the necessary blocks of the actual disk file.
}

/*
 This function empties a block as specified by the first arguement in the memory copy of the disk file.
*/
void emptyBlock(int blockNo) 
{
	int i;
	for(i = 0 ; i < BLOCK_SIZE ; i++)
	{
		strcpy(disk[blockNo].word[i],"") ;
	}
}


/*
	This function frees the blocks specified by the block number present in the first arguement. The second arguement is the size
	of the first argument.
	The memory copy is not committed.
*/
void freeUnusedBlock(int *freeBlock, int size){
	int i=0;
	for( i = 0 ; i < size && freeBlock[i] != -1 && freeBlock[i] != 0; i++){
		//printf("Block Num = %d\nLocation = %d", freeBlock[i],freeBlock[i] % BLOCK_SIZE );
		storeValueAt(DISK_FREE_LIST * BLOCK_SIZE + freeBlock[i] , 0 );
		emptyBlock(TEMP_BLOCK);
		writeToDisk(TEMP_BLOCK,freeBlock[i]);//clear the contents of the block in disk
	}
}


/*
	This function returns the address of a free block on the disk.
	The value returned will be the relative word address of the corresponding entry in the free list.
*/
int FindFreeBlock(){
	int i,j;
	for(i = DISK_FREE_LIST ; i < DISK_FREE_LIST + NO_OF_FREE_LIST_BLOCKS ;i++){
		for(j = 0 ; j < BLOCK_SIZE; j++){
			if( getValue(disk[i].word[j]) == 0 ){
				storeValue( disk[i].word[j] , 1 );	
				return ((i-DISK_FREE_LIST)*BLOCK_SIZE + j);
			}
		}
	}
	return -1;	
}

/*
	Fills the mem copy of disk Data Structure with default values
*/
void setDefaultValues(int dataStructure)
{
	int i,j;
	switch(dataStructure)
	{
		case DISK_FREE_LIST:
			for(j=0; j<(NO_OF_FREE_LIST_BLOCKS*BLOCK_SIZE); j++)
			{
				i=j/BLOCK_SIZE;
				if( (j>=DATA_START_BLOCK) && (j<NO_OF_DISK_BLOCKS ))
					storeValue(disk[DISK_FREE_LIST+i].word[j], 0);//Fill data block entries with 0(Free block)
				else
					storeValue(disk[DISK_FREE_LIST+i].word[j], 1);
			}
			break;

		case INODE:
			for(j=0; j<NO_OF_INODE_BLOCKS; j++)
			{
				for(i=0; i<BLOCK_SIZE; i++)
					storeValue(disk[INODE + j].word[i], -1);//Enters -1 to all INODE ENTRIES

				for(i=0; i<BLOCK_SIZE; i+=INODE_ENTRY_SIZE)
					storeValue(disk[INODE + j].word[INODE_ENTRY_FILESIZE + i],0);//Set 0 to filesize of all entries

				for(i=0; i<BLOCK_SIZE; i+=INODE_ENTRY_SIZE)
					storeValue(disk[INODE + j].word[INODE_ENTRY_FILENAME + i],-1);//Set -1 to filename of all entries
			}
			break;

		case ROOTFILE:
			for(j=0; j<NO_OF_ROOTFILE_BLOCKS; j++)
			{
				for(i=0; i<BLOCK_SIZE; i++)
					storeValue(disk[ROOTFILE + j].word[i], -1);//Enters -1 to all INODE ENTRIES

				for(i=0; i<BLOCK_SIZE; i+=ROOTFILE_ENTRY_SIZE)
					storeValue(disk[ROOTFILE + j].word[ROOTFILE_ENTRY_FILESIZE + i],0);//Set 0 to filesize of all entries

				for(i=0; i<BLOCK_SIZE; i+=ROOTFILE_ENTRY_SIZE)
					storeValue(disk[ROOTFILE + j].word[ROOTFILE_ENTRY_FILENAME + i],-1);//Set -1 to filename of all entries
			}
			break;
	}
}

/*
	Commits the Memory copy of the disk dataStructure specified to the disk file
*/
void commitMemCopyToDisk(int dataStructure)
{
	int i;
	switch(dataStructure)
	{
		case DISK_FREE_LIST:
			for(i=0; i < NO_OF_FREE_LIST_BLOCKS; i++)
				writeToDisk(DISK_FREE_LIST + i, DISK_FREE_LIST + i);
			break;

		case INODE://in case of inode, root file is also committed along with it
			for(i=0; i<NO_OF_INODE_BLOCKS; i++)
				writeToDisk(INODE + i, INODE + i);
		case ROOTFILE:
			for(i=0; i<NO_OF_ROOTFILE_BLOCKS; i++)
				writeToDisk(ROOTFILE + i, ROOTFILE + i);
			break;
	}
}


/*
	This function lists all the files present on the disk.
	This is done as follows:
	1. The basic block entry in the memory copy of the disk is searched. If the value is not -1 then the filename is 
	shown as output.
*/
XOSFILE* _getAllFiles(){
	diskCheckFileExists();
	
	int i,j;
	XOSFILE *sentinel, *curr_ptr;
	int hasFiles = 0; 	// Flag which indicates if disk has no files

	/* The sentinel works as a sentinel. */
	sentinel = malloc(sizeof(XOSFILE));
	sentinel->next = NULL;
	curr_ptr = sentinel;

	for(j = INODE ; j < INODE + NO_OF_INODE_BLOCKS ; j++)
	{
		for(i = 0 ; i < BLOCK_SIZE ; i = i + INODE_ENTRY_SIZE)
		{
			if( getValue(disk[j].word[INODE_ENTRY_FILENAME + i]) != -1 )	// -1 indicates invalid INODE
			{ 	
				hasFiles = 1;
				XOSFILE *new_entry;

				new_entry = malloc (sizeof(XOSFILE));
				new_entry->name = strdup(disk[j].word[i + INODE_ENTRY_FILENAME]);
				new_entry->size = getValue(disk[j].word[i + INODE_ENTRY_FILESIZE]);
				curr_ptr->next = new_entry;
				curr_ptr = new_entry;
				curr_ptr->next = NULL;
			}		
		}
	}
	
	curr_ptr = sentinel->next;
	free(sentinel);
	return curr_ptr;
}

/*
	This function initialises the memory copy of the disk with the contents from the actual disk file.
*/
int loadFileToVirtualDisk()
{
	int i;
	for(i=DISK_FREE_LIST; i<DISK_FREE_LIST + NO_OF_FREE_LIST_BLOCKS; i++)
		readFromDisk(i,i);
	for(i=INODE; i<INODE + NO_OF_INODE_BLOCKS; i++)
		readFromDisk(i,i);
	for(i=ROOTFILE; i<ROOTFILE + NO_OF_ROOTFILE_BLOCKS; i++)
		readFromDisk(i,i);	
}

/*
	This function wipes out the entire contents of the memory copy of the disk.
*/
void clearVirtDisk()
{
	bzero(disk, sizeof(disk));
}

/*
	char* to int conversion
	get integer value at address
*/
int getValueAt(int address)
{
	getValue( disk[(address / BLOCK_SIZE)].word[(address % BLOCK_SIZE)]);
}

/*
	char* to int conversion
 */

int getValue(char* str ) 
{
	return atoi(str);
}

/*
	int to char* conversion
*/
void storeValueAt(int address, int num) 
{
	storeValue( disk[(address / BLOCK_SIZE)].word[(address % BLOCK_SIZE)] , num );
}

void storeValue(char *str, int num) 
{
	sprintf(str,"%d",num);
}

void storeStringValueAt(int address, char *value) 
{
	strcpy( disk[(address / BLOCK_SIZE)].word[(address % BLOCK_SIZE)] , value );
}
