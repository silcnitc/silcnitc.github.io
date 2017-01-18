#ifndef FILESYS_H
#define FILESYS_H

#define BLOCK_SIZE 512
#define WORD_SIZE 16

#define OS_STARTUP_CODE 0
#define DISK_FREE_LIST 2
#define INODE 3
#define ROOTFILE 5
#define INIT_BLOCK 7
#define SHELL_BLOCK 9
#define IDLE_BLOCK 11
#define LIBRARY_BLOCK 13
#define EX_HANDLER 15
#define TIMERINT 17
#define DISKCONTROLLER_INT 19
#define CONSOLE_INT 21
#define INT0 EX_HANDLER
#define INT1 TIMERINT
#define INT2 DISKCONTROLLER_INT
#define INT3 CONSOLE_INT
#define INT4 23
#define INT5 25
#define INT6 27
#define INT7 29
#define INT8 31
#define INT9 33
#define INT10 35
#define INT11 37
#define INT12 39
#define INT13 41
#define INT14 43
#define INT15 45
#define INT16 47
#define INT17 49
#define INT18 51
#define MOD0 53
#define MOD1 55
#define MOD2 57
#define MOD3 59
#define MOD4 61
#define MOD5 63
#define MOD6 65
#define MOD7 67


#define OS_STARTUP_CODE_SIZE 2
#define NO_OF_FREE_LIST_BLOCKS 1
#define NO_OF_ROOTFILE_BLOCKS 1
#define NO_OF_INIT_BLOCKS 4
#define NO_OF_SHELL_BLOCKS 2
#define NO_OF_IDLE_BLOCKS 2
#define NO_OF_LIBRARY_BLOCKS 2
#define EX_HANDLER_SIZE 2
#define TIMERINT_SIZE 2
#define DISKCONTROLLER_INT_SIZE 2
#define CONSOLE_INT_SIZE 2
#define INT0_SIZE EX_HANDLER_SIZE
#define INT1_SIZE TIMERINT_SIZE
#define INT2_SIZE DISKCONTROLLER_INT_SIZE
#define INT3_SIZE CONSOLE_INT_SIZE
#define INT4_SIZE 2
#define INT5_SIZE 2
#define INT6_SIZE 2
#define INT7_SIZE 2
#define INT8_SIZE 2
#define INT9_SIZE 2
#define INT10_SIZE 2
#define INT11_SIZE 2
#define INT12_SIZE 2
#define INT13_SIZE 2
#define INT14_SIZE 2
#define INT15_SIZE 2
#define INT16_SIZE 2
#define INT17_SIZE 2
#define INT18_SIZE 2
#define INT_SIZE INT4_SIZE
#define MOD0_SIZE 2
#define MOD1_SIZE 2
#define MOD2_SIZE 2
#define MOD3_SIZE 2
#define MOD4_SIZE 2
#define MOD5_SIZE 2
#define MOD6_SIZE 2
#define MOD7_SIZE 2
#define MOD_SIZE MOD0_SIZE

#define INODE 3
#define NO_OF_INODE_BLOCKS 2

#define NO_OF_INTERRUPTS 18
#define NO_OF_MODULES 8

#define DATA_START_BLOCK 69
#define NO_OF_DATA_BLOCKS 187

#define SWAP_START_BLOCK 256
#define NO_OF_SWAP_BLOCKS 256

#define NO_OF_DISK_BLOCKS 512

#define DISK_SIZE (NO_OF_DISK_BLOCKS * BLOCK_SIZE)


/*
Declarations for INODE Entry
*/ 

#define INODE_MAX_FILE_NUM 60
#define INODE_MAX_BLOCK_NUM 4

#define INODE_ENTRY_FILETYPE 0
#define INODE_ENTRY_FILENAME 1
#define INODE_ENTRY_FILESIZE 2
#define INODE_ENTRY_USERID 3
#define INODE_ENTRY_PERMISSION 4
#define INODE_ENTRY_DATABLOCK 8
#define INODE_NUM_DATA_BLOCKS INODE_MAX_BLOCK_NUM
#define INODE_ENTRY_SIZE 16
#define INODE_SIZE (NO_OF_INODE_BLOCKS * BLOCK_SIZE)

#define FILETYPE_ROOT 1
#define FILETYPE_DATA 2
#define FILETYPE_EXEC 3 //TODO check whether these are used


/*
	Root File Declarations
*/

#define ROOTFILE_ENTRY_FILENAME 0
#define ROOTFILE_ENTRY_FILESIZE 1
#define ROOTFILE_ENTRY_FILETYPE 2
#define ROOTFILE_ENTRY_USERNAME 3
#define ROOTFILE_ENTRY_PERMISSION 4
#define ROOTFILE_ENTRY_SIZE 8


/*
Other declarations
*/

#define NO_BLOCKS_TO_COPY 69		//Rest of the blocks have data. 
#define EXTRA_BLOCKS	1			// Need a temporary block
#define TEMP_BLOCK 69				//Temporary block no: starting from 0.

typedef struct _XOSFILE
{
	char *name;
	int size;

	struct _XOSFILE *next;
} XOSFILE;

#endif
