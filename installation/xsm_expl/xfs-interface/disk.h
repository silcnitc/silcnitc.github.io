#ifndef DISK_H
#define DISK_H

#include "fileSystem.h"

#define DISK_NAME 	"disk.xfs"
#define BOOT_BLOCK	0

#define DISK_NO_FORMAT 0
#define DISK_FORMAT 1

typedef struct{
	char word[BLOCK_SIZE][WORD_SIZE];
}BLOCK;

int readFromDisk(int virtBlockNumber, int fileBlockNumber);
int writeToDisk(int virtBlockNumber, int fileBlockNumber);
int openDiskFile(int access);
void createDiskFile(int format);
void diskCheckFileExists();

#endif