#ifndef DISKUTILS_H
#define DISKUTILS_H
#include <string.h>
#include "fileSystem.h"

void _getAllFilesdisk_init();
void emptyBlock(int blockNo);
void freeUnusedBlock(int *freeBlock, int size);
int FindFreeBlock();
void setDefaultValues(int dataStructure);
void commitMemCopyToDisk(int dataStructure);
XOSFILE* _getAllFiles();
int loadFileToVirtualDisk();
void clearVirtDisk();
int getValueAt(int address);
int getValue(char* str );
void storeValueAt(int address, int num);
void storeValue(char *str, int num);
void storeStringValueAt(int address, char *value);

#endif
