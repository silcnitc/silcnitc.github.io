#include "disk.h"
#include "exception.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

BLOCK* disk;

/*
 This function reads an entire BLOCK from the address specified from fileBlockNumber on the disk file to virtBlockNumber on the memory copy of the disk.
*/
int readFromDisk(int virtBlockNumber, int fileBlockNumber) {
	int fd;
	fd = openDiskFile(O_RDONLY);
	lseek(fd,sizeof (BLOCK)*fileBlockNumber,SEEK_SET);
	read(fd,&disk[virtBlockNumber],sizeof (BLOCK));
	close(fd);
	return 0;
}


/*
  This function writes an entire block to fileBlocknumber on the disk file from virtBlockNumber on the memory copy of the disk.
*/
int writeToDisk(int virtBlockNumber, int fileBlockNumber) {
	int fd;
	fd = openDiskFile(O_WRONLY);
	lseek(fd,0,SEEK_SET);
	lseek(fd,sizeof (BLOCK)*fileBlockNumber,SEEK_CUR);
	write(fd,&disk[virtBlockNumber],sizeof (BLOCK));
	close(fd);	
	return 0;
}

/*
	Opens the disk file returns the file descriptor
	if it fails, throws an exception and transfers control to the interface
*/
int openDiskFile(int access)
{
	int fd;
	fd = open(DISK_NAME, access, 0666);
	if(fd < 0){
	  exception_throwException(EXCEPTION_CANT_OPEN_DISK);
	}
	return fd;
}

/*
	Creates the disk file
	Arguments: format) disk is formatted if its value is DISK_FORMAT
	if it fails, throws an exception and transfers control to the interface
*/
void createDiskFile(int format)
{
	int fd;

	if(format==DISK_FORMAT)
		fd = open(DISK_NAME, O_CREAT | O_TRUNC | O_SYNC, 0666);
	else
		fd = open(DISK_NAME, O_CREAT, 0666);

	if(fd < 0){
	  exception_throwException(EXCEPTION_CANT_CREATE_DISK);
	}
	close(fd);
}

/*
	Tries to open the disk file, throws an exception if it fails.
	An exception returns the control to the jmppoint set in interface.c
*/
void diskCheckFileExists()
{
	close(openDiskFile(O_RDONLY));
}
