#ifndef DISKUTIL_H
#define DISKUTIL_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "disk.h"


#define ASSEMBLY_CODE 0
#define DATA_FILE 1

#define XFS_ERROR -1



XOSFILE* getAllFiles();
void listAllFiles();
void disk_init();
int deleteExecutableFromDisk(char *name);
int loadExecutableToDisk(char *name);
int loadCode();
int loadCodeWithLabels(char* infile, int disk_block, int no_of_disk_blocks, int mem_page);
int loadOSCode(char* name);
int getDataFileSize(FILE *fp);
int loadIntCode(char* name, int intNo);
int loadModuleCode(char* infile, int modNo);
int loadTimerCode(char* name);
int loadDiskControllerIntCode(char* name);
int loadConsoleIntCode(char* name);
int loadExHandlerToDisk(char* name);
int loadINITCode(char* name);
void displayFileContents(char *name);
int clearDiskBlocks();
int deleteINITFromDisk();
int deleteOSCodeFromDisk();
int deleteTimerFromDisk();
int deleteDiskControllerINTFromDisk();
int deleteConsoleINTFromDisk();
int deleteIntCode(int intNo);	
int deleteExHandlerFromDisk();
void displayDiskFreeList();
int loadDataToDisk(char *name);
int deleteDataFromDisk(char *name);
void formatDisk(int format);
void expandpath(char *path);
void addext(char *filename, char *ext);
#endif
