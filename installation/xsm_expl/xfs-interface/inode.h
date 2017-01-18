#ifndef INODE_H
#define INODE_H

/*
  This function removes the inode entry corresponding to the first arguement.
*/
int removeInodeEntry(int locationOfInode);

/*
  This function returns the basic block entry(pass by pointer) corresponding to the address specified by the second arguement.
*/
int getDataBlocks(int *basicBlockAddr, int locationOfInode);


/*
  This function returns an  empty inode entry if present.
*/
int FindEmptyInodeEntry();

/*
  This function adds the name, size and basic block address of the file to corresponding entry in the inode.
*/
void AddEntryToMemInode(int startIndexInInode, int fileType, char *nameOfFile, int size_of_file, int* addrOfDataBlocks);

int getInodeEntry(char *name);

#endif