#include "fileSystem.h"
#include "memOrg.h"
#include "inode.h"
#include "diskUtility.h"
#include "virtualDisk.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern BLOCK* disk;

void disk_init()
{
	_disk_init();
}

void
listAllFiles ()
{
	XOSFILE *list, *next;

	list = getAllFiles();

	if (!list)
	{
		printf("The disk contains no files.\n");
		return;
	}

	while (list)
	{
		printf ("Filename: %s Filesize %d\n", list->name, list->size);
		next = list->next;

		free (list->name);
		free(list);
		list = next;
	}
}

XOSFILE* getAllFiles()
{
	return _getAllFiles();
}

/*
	This function deletes an executable/data file from the disk.
	NOTE: 1. Memory copy is committed to disk.
	2. Due to a technical glitch any string which is already stored on the disk will have to be searched in the
		memory copy after appending a newline.
*/
int deleteFileFromDisk(char *name)
{
	diskCheckFileExists();

	int max_num_blocks = INODE_MAX_BLOCK_NUM;
	int locationOfInode,i,blockAddresses[max_num_blocks];	//0,1,2,3-code+data blocks
	for(i = 0; i < max_num_blocks; i++)
		blockAddresses[i]=0;
	locationOfInode = getInodeEntry(name);
	if(locationOfInode < 0){
		printf("File \'%s\' not found!\n",name);
		return -1;
	}
	
	getDataBlocks(blockAddresses,locationOfInode);		
	freeUnusedBlock(blockAddresses, max_num_blocks);
	
	removeInodeEntry(locationOfInode);
	
	commitMemCopyToDisk(INODE);
	commitMemCopyToDisk(DISK_FREE_LIST);
	
	return 0;	
}

/*
	This function clears the contents of disk blocks specified
*/
int clearDiskBlocks(int disk_start_block, int no_of_disk_blocks)
{
	int i;
	emptyBlock(TEMP_BLOCK);
	for (i=0; i<no_of_disk_blocks; i++)
		writeToDisk(TEMP_BLOCK, disk_start_block + i);	
	return 0;
}

/*
	This function deletes the INIT code from the disk.
*/
int deleteINITFromDisk()
{
	return clearDiskBlocks(INIT_BLOCK, NO_OF_INIT_BLOCKS);
}

/*
	This function deletes the OS code from the disk.
*/
int deleteOSCodeFromDisk()
{
	return clearDiskBlocks(OS_STARTUP_CODE, OS_STARTUP_CODE_SIZE);
}

/*
	This function deletes the Timer Interrupt from the disk.
*/
int deleteTimerFromDisk()
{
	return clearDiskBlocks(TIMERINT, TIMERINT_SIZE);
}

int deleteDiskControllerINTFromDisk()
{
	return clearDiskBlocks(DISKCONTROLLER_INT, DISKCONTROLLER_INT_SIZE);
}

int deleteConsoleINTFromDisk()
{
	return clearDiskBlocks(CONSOLE_INT, CONSOLE_INT_SIZE);
}
/*
	This function deletes the Interrupt <intNo> from the disk.
*/
int deleteIntCode(int intNo)
{
	return clearDiskBlocks(((intNo - 1) * INT1_SIZE) + INT1, INT1_SIZE);
}

/*
	This function deletes the Exception Handler from the disk.
*/
int deleteExHandlerFromDisk()
{
	return clearDiskBlocks(EX_HANDLER, EX_HANDLER_SIZE);
}


/*
	This file copies the necessary contents of a file to the corresponding location specified by the second arguemnt on the disk.
	The file is first copied to the memory copy of the disk. This is then committed to the actual disk file.
	NOTE: 1. EOF is set only after reading beyond the end of the file. This is the reason why the if condition is needed is needed.
	2. Also the function must read till EOF or BLOCK_SIZE line so that successive read proceeds accordingly
*/
int writeFileToDisk(FILE *fp, int blockNum, int type)
{
	
	int i, line=0,j;
	char buffer[32],s[16],temp[100],c;
	emptyBlock(TEMP_BLOCK);
	if(type==ASSEMBLY_CODE)			//writing files with assembly code
	{
		char *instr, *arg1, *arg2, *string_start;
		int line_count=0,flag=0,k=0;
		for(i = 0; i < (BLOCK_SIZE/2); i++)
		{
			
			fgets(temp,100,fp);
			
			if(feof(fp)){
				strcpy(disk[TEMP_BLOCK].word[line_count], "");
				writeToDisk(TEMP_BLOCK,blockNum);
				return -1;
			 }
			 
			//printf("DET #%s#\n",temp);
			
			string_start=strchr(temp,'"');
			if(string_start==NULL)
			{
				for(k=0;k<31;k++)
					buffer[k]=temp[k];
				buffer[k]='\0';
			}
			else
			{
				if(strlen(string_start)<=16)
				{
					for(k=0;k<31;k++)
						buffer[k]=temp[k];
					buffer[k]='\0';
				}
				else
				{
					for(k=0;k<(strlen(temp)-strlen(string_start)+15);k++)
					{
						buffer[k]=temp[k];
					}
					buffer[k-1]='"';
					buffer[k]='\0';
				}
			}
		
			
			if(strlen(buffer)>1)
			{
				if(buffer[strlen(buffer)-1]=='\n')
					buffer[strlen(buffer)-1]='\0';
				instr=strtok(buffer," ");
				arg1=strtok(NULL," ");
				arg2=strtok(NULL,",");
			
				bzero(s,16);
				if(arg1!=NULL)
				{
					sprintf(s,"%s %s",instr,arg1);
					for(j=strlen(s);j<16;j++)
						s[j]='\0';
					strcpy(disk[TEMP_BLOCK].word[line_count],s);
					if(arg2!=NULL)
					{
						strcpy(s,arg2);
						for(j=strlen(s);j<16;j++)
							s[j]='\0';
						strcpy(disk[TEMP_BLOCK].word[line_count+1],s);
				
					}
					else
					{
						for(j=0;j<16;j++)
							s[j]='\0';
						strcpy(disk[TEMP_BLOCK].word[line_count+1],s);
					}
					line_count=line_count+2;
				}
				else
				{
					sprintf(s,"%s",instr);
					for(j=strlen(s);j<=16;j++)
						strcat(s,"\0");
					strcpy(disk[TEMP_BLOCK].word[line_count],s);
					bzero(s,16);
					for(j=0;j<16;j++)
						s[j]='\0';
					strcpy(disk[TEMP_BLOCK].word[line_count+1],s);
					line_count=line_count+2;
			
				}
			
			}
			
		}
		writeToDisk(TEMP_BLOCK,blockNum);
		return 1;
	}	
	else if(type==DATA_FILE)			//writing data files
	{
		char buffer1[16],c;
		for(i = 0; i < BLOCK_SIZE; i++)
		{
			fgets(buffer1,16,fp);
			strcpy(disk[TEMP_BLOCK].word[i],buffer1);
			if(feof(fp))
			{
				strcpy(disk[TEMP_BLOCK].word[i], "");
				writeToDisk(TEMP_BLOCK,blockNum);
				return -1;
			}	
		}
		writeToDisk(TEMP_BLOCK,blockNum);
		return 1;
	}

}


/*
	This function loads the executable file corresponding to the first arguement to an appropriate location on the disk.
	This function systematically uses the above functions to do this action.
*/
int loadExecutableToDisk(char *name)
{
	FILE *fileToBeLoaded;
	int freeBlock[INODE_MAX_BLOCK_NUM];
	int i,j,k,l,file_size=0,num_of_lines=0,num_of_blocks_reqd=0;
	for(i=0;i<INODE_MAX_BLOCK_NUM;i++)
		freeBlock[i]=-1;
	char c='\0',*s;
	char filename[50];
	s = strrchr(name,'/');
	if(s!=NULL)
		strcpy(filename,s+1);
	else
		strcpy(filename,name);	
	
	filename[15]='\0';
		
	addext(filename,".xsm");

	expandpath(name);
	fileToBeLoaded = fopen(name, "r");
	if(fileToBeLoaded == NULL){
		printf("File %s not found.\n", name);
		return -1;
	}
	if(fileToBeLoaded == NULL){
		printf("The file could not be opened");
		return -1;
	}
	
	while(c!=EOF)
	{
		c=fgetc(fileToBeLoaded);
		if(c=='\n')
			num_of_lines++;
	}
	
	num_of_blocks_reqd = (num_of_lines / (BLOCK_SIZE/2)) + 1;
	
	if(num_of_blocks_reqd > INODE_MAX_BLOCK_NUM)
	{
		printf("The size of file exceeds %d blocks",INODE_MAX_BLOCK_NUM);
		return -1;
	}
	
	fseek(fileToBeLoaded,0,SEEK_SET);
	
	for(i = 0; i < num_of_blocks_reqd; i++)
	{
		if((freeBlock[i] = FindFreeBlock()) == -1){
				printf("Insufficient disk space!\n");
				freeUnusedBlock(freeBlock, INODE_MAX_BLOCK_NUM);
				return -1;
			}
	}
	i = getInodeEntry(filename);
	if( i >= 0 ){
		printf("Disk already contains the file with this name. Try again with a different name.\n");
		freeUnusedBlock(freeBlock, INODE_MAX_BLOCK_NUM);
		return -1;
	}
	
	k = FindEmptyInodeEntry();	
		
	if( k == -1 ){
		freeUnusedBlock(freeBlock, INODE_MAX_BLOCK_NUM);
		printf("No free INODE entry found.\n");
		return -1;			
	}
	
	commitMemCopyToDisk(DISK_FREE_LIST);
	emptyBlock(TEMP_BLOCK);				//note:need to modify this
	
	for(i=0;i<num_of_blocks_reqd;i++)
	{
		j = writeFileToDisk(fileToBeLoaded, freeBlock[i], ASSEMBLY_CODE);
		file_size++;
	}
	
	AddEntryToMemInode(k, FILETYPE_EXEC,filename, file_size * BLOCK_SIZE, freeBlock);	
	
	commitMemCopyToDisk(INODE);
	
	close(fileToBeLoaded);
	return 0;
}



/*
	This function loads a data file to the disk.
*/
int loadDataToDisk(char *name)
{
	FILE *fileToBeLoaded;
	int freeBlock[INODE_MAX_BLOCK_NUM];
	int i,j,k,num_of_chars=0,num_of_blocks_reqd=0,num_of_words,file_size=0;
	for(i=0;i<INODE_MAX_BLOCK_NUM;i++)
		freeBlock[i]=-1;
	char c='\0',*s;
	char filename[50];
	s = strrchr(name,'/');
	if(s!=NULL)
		strcpy(filename,s+1);
	else
		strcpy(filename,name);	
	
	filename[15]='\0';
	addext(filename,".dat");

	expandpath(name);
	fileToBeLoaded = fopen(name, "r");
	if(fileToBeLoaded == NULL)
	{
		printf("File \'%s\' not found.!\n", name);
		return -1;
	}
	if(fileToBeLoaded == NULL)
	{
		printf("The file could not be opened!");
		return -1;
	}
	
	fseek(fileToBeLoaded, 0L, SEEK_END);
	
	num_of_chars = ftell(fileToBeLoaded);
	num_of_words = getDataFileSize(fileToBeLoaded);
	num_of_blocks_reqd = (num_of_words / 512) + ((num_of_words%512==0)?0:1);
	//printf("\n Chars = %d, Words = %d, Blocks(chars) = %d, Blocks(words) = %d\n",num_of_chars,num_of_words,num_of_blocks_reqd,(num_of_words/512));
	if(num_of_blocks_reqd > INODE_MAX_BLOCK_NUM)
	{
		printf("The size of file exceeds %d blocks\n",INODE_MAX_BLOCK_NUM);
		printf("The file contains %d words, an xfs file can have only upto %d words\n", num_of_words, INODE_MAX_BLOCK_NUM * BLOCK_SIZE);
		return -1;
	}
	
	fseek(fileToBeLoaded,0,SEEK_SET);
	
	for(i = 0; i < num_of_blocks_reqd; i++)
	{
		if((freeBlock[i] = FindFreeBlock()) == -1){
				printf("Disk does not have enough space to contain the file.\n");
				freeUnusedBlock(freeBlock, INODE_MAX_BLOCK_NUM);
				return -1;
			}
	}

	i = getInodeEntry(filename);
	if( i >= 0 )
	{
		printf("Disk already contains the file with this name. Try again with a different name.\n");
		freeUnusedBlock(freeBlock, INODE_MAX_BLOCK_NUM);
		return -1;
	}
	
	k = FindEmptyInodeEntry();
			
	if( k == -1 )
	{
		freeUnusedBlock(freeBlock, INODE_MAX_BLOCK_NUM);
		printf("No free INODE entry found.\n");
		return -1;			
	}
	
	commitMemCopyToDisk(DISK_FREE_LIST);

	emptyBlock(TEMP_BLOCK);				//note:need to modify this
	
	
	for(i=0;i<num_of_blocks_reqd;i++)//load the file
	{
		j = writeFileToDisk(fileToBeLoaded, freeBlock[i], DATA_FILE);
		file_size++;
	}
	
	AddEntryToMemInode(k, FILETYPE_DATA, filename, file_size * BLOCK_SIZE, freeBlock);		
	commitMemCopyToDisk(INODE);
	
	close(fileToBeLoaded);
	return 0;
}

/*
	Returns the size of a unix data file in words
*/
int getDataFileSize(FILE *fp)
{
	int num_of_words=0;
	char buf[XSM_WORD_SIZE];
	fseek(fp,0,SEEK_SET);
	while(1)
	{
		fgets(buf,XSM_WORD_SIZE,fp);
		num_of_words++;
		if(feof(fp))
			break;
	}
	return num_of_words;
}

int loadCodeWithLabels(char* infile, int disk_block, int no_of_disk_blocks, int mem_page)
{
	expandpath(infile);
	
	char fileName[66];
	int ret;

	labels_reset ();	
	labels_resolve (infile, fileName, mem_page * PAGE_SIZE);

	ret=loadCode(fileName, disk_block, no_of_disk_blocks);

	if(remove(fileName))
	{
		printf("Error while trying to delete temporary file\n");
	}

	return ret;
}

/*
	This function loads the code specified by the first argument to the specified location on disk.
	labels in code are replaced by mem addresses, taking the given MEM_PAGE as memory base address
	The code is first copied to memory copy. If this copying proceeds properly then the memory copy is committed to the disk.
*/
int loadCode(char* fileName, int disk_start_block, int no_of_disk_blocks)
{
	FILE* fp = fopen(fileName, "r");
	int i,j;
	if(fp == NULL)
	{
		printf("File %s not found.\n", fileName);
		return -1;
	}
	
	for(i=0;i<no_of_disk_blocks;i++)
	{
		j = writeFileToDisk(fp, disk_start_block + i, ASSEMBLY_CODE);
		if (j != 1)
			break;
	}
	if(j==1)
	{
		printf("Code exceeds %d block\n",no_of_disk_blocks);
		clearDiskBlocks(disk_start_block,no_of_disk_blocks);
	}
	close(fp);
	return 0;
}

/*
	This function copies the init program to its proper location on the disk.
*/
int loadINITCode(char* infile )
{
	return loadCode(infile, INIT_BLOCK, NO_OF_INIT_BLOCKS);
}

/*
	This function copies the idle program to its proper location on the disk.
*/
int loadIdleCode(char* infile )
{
	return loadCode(infile, IDLE_BLOCK, NO_OF_IDLE_BLOCKS);
}

/*
	This function copies the shell program to its proper location on the disk.
*/
int loadShellCode(char* infile )
{
	return loadCode(infile, SHELL_BLOCK, NO_OF_SHELL_BLOCKS);
}

/*
	This function copies the library to its proper location on the disk.
*/
int loadLibraryCode(char* infile )
{
	return loadCodeWithLabels(infile, LIBRARY_BLOCK, NO_OF_LIBRARY_BLOCKS, MEM_LIBRARY_PAGE);
}

/*
	This function loads the OS startup code specified by the first argument to its appropriate location on disk.
	The code is first copied to memory copy. If this copying proceeds properly then the memory copy is committed to the disk.
*/
int loadOSCode(char* infile)
{
	return loadCodeWithLabels(infile, OS_STARTUP_CODE, OS_STARTUP_CODE_SIZE, MEM_OS_STARTUP_CODE);
}

int loadDiskControllerIntCode(char* infile)
{
	return loadCodeWithLabels(infile, DISKCONTROLLER_INT, DISKCONTROLLER_INT_SIZE, MEM_DISKCONTROLLER_INT);
}

int loadConsoleIntCode(char* infile)
{
	return loadCodeWithLabels(infile, CONSOLE_INT, CONSOLE_INT_SIZE, MEM_CONSOLE_INT);
}

/*
	This function copies the interrupts to the proper location on the disk.
*/
int loadIntCode(char* infile, int intNo)
{
	return loadCodeWithLabels(infile,((intNo - 1) * INT_SIZE) + INT1, INT_SIZE, ((intNo - 1) * MEM_INT_SIZE) + MEM_INT1 );
}

/*
	This function copies the module code to the proper location on the disk.
*/
int loadModuleCode(char* infile, int modNo)
{
	return loadCodeWithLabels(infile,(modNo * MOD_SIZE) + MOD0, MOD_SIZE, (modNo * MEM_MOD_SIZE) + MEM_MOD0 );
}
/*
	This function copies the timer interrupt to the proper location on the disk.
*/
int loadTimerCode(char* infile)
{
	return loadCodeWithLabels(infile, TIMERINT, TIMERINT_SIZE, MEM_TIMERINT);
}

/*
	This function copies the exception handler to the proper location on the disk.
*/
int loadExHandlerToDisk(char* infile)
{
	return loadCodeWithLabels(infile, EX_HANDLER, EX_HANDLER_SIZE, MEM_EX_HANDLER);
}

/*
	This function displays the content of the files stored in the disk.
*/
void displayFileContents(char *name)
{
	diskCheckFileExists();
	int i,j,k,l,flag=0,locationOfInode;
	int blk[INODE_NUM_DATA_BLOCKS];
	
	for(i=0;i<INODE_NUM_DATA_BLOCKS;i++)
		blk[i] = 0;
	
	locationOfInode = getInodeEntry(name);
	if(locationOfInode < 0){
		printf("File \'%s\' not found!\n",name);
		return;
	}
		
	getDataBlocks(blk,locationOfInode);

	k = 0;
	while (blk[k] > 0)
	{
		emptyBlock(TEMP_BLOCK);
		readFromDisk(TEMP_BLOCK,blk[k]);
		for(l=0;l<BLOCK_SIZE;l++)
		{
			if(strcmp(disk[TEMP_BLOCK].word[l],"\0")!=0)
				printf("%s	\n",disk[TEMP_BLOCK].word[l]);
		}
		//printf("next block\n");
		emptyBlock(TEMP_BLOCK);
		k++;
	}
}

/*
	This function copies the contents of the disk starting from <startBlock> to <endBlock> to a unix file.
*/
int copyBlocksToFile (int startblock,int endblock,char *filename)
{
	diskCheckFileExists();

	int i,j;
	FILE *fp;
	expandpath(filename);
	fp = fopen(filename,"w");
	if(fp == NULL)
	{
		printf("File \'%s\' not found!\n", filename);
		return -1;
	}
	else
	{
		for(i = startblock; i <= endblock; i++)
		{
			emptyBlock(TEMP_BLOCK);
			readFromDisk(TEMP_BLOCK,i);
			for(j=0;j<BLOCK_SIZE;j++)
			{
				fprintf(fp,"%s\n",disk[TEMP_BLOCK].word[j]);
			}
		}
		fclose(fp);
	}
	return 0;
}

/*
	This function displays disk free list and the amount of free space in the disk.
*/
void displayDiskFreeList()
{
	diskCheckFileExists();
	
	int i,j,no_of_free_blocks=0;
	for(j = 0; j < NO_OF_FREE_LIST_BLOCKS; j++)
	{
		for(i = 0; i < BLOCK_SIZE; i++)
		{
			printf("%d \t - \t %s  \n",i,disk[DISK_FREE_LIST+j].word[i]);
			if(getValue(disk[DISK_FREE_LIST+j].word[i])==0)
				no_of_free_blocks++;
		}
	}
	printf("\nNo of Free Blocks = %d",no_of_free_blocks);
	printf("\nTotal no of Blocks = %d\n",NO_OF_DISK_BLOCKS);
}

/*
	formatDisk creates	the disk file if not present.
	if format is equal to zero then the function creates the disk but does not format it.
	if format is not equal to zero then the function will create and format the disk.
	Formatting is done as follows:
	1. A memory copy of the disk is maintained. This copy contains NO_BLOCKS_TO_COPY + EXTRA_BLOCKS (in this case 13 + 1) blocks.
	The extra block is a temporary block. This memory copy is called the virtual disk. This is first cleared.
	2. Then the memory freelist is initialised.
	3. The fat blocks are also initialised. The basic block entries are all set to -1. The memory copy is then committed to the 
	disk file.
	4. Finally the entry for init process is made.
*/
void formatDisk(int format)
{
	int fd,rootFileDataBlocks[INODE_NUM_DATA_BLOCKS];
	if(format)
	{
		createDiskFile(DISK_NO_FORMAT);
		clearVirtDisk();
		int i=0;
		
		setDefaultValues(DISK_FREE_LIST);
		commitMemCopyToDisk(DISK_FREE_LIST);
		
		setDefaultValues(INODE);
		setDefaultValues(ROOTFILE);

		for(i = 0; i < INODE_NUM_DATA_BLOCKS; i++)//Add root file blocks address to an array
		{
			if(i < NO_OF_ROOTFILE_BLOCKS)
				rootFileDataBlocks[i] = ROOTFILE + i;
			else
				rootFileDataBlocks[i] = -1;
		}

		AddEntryToMemInode(0, FILETYPE_ROOT, "root", NO_OF_ROOTFILE_BLOCKS * BLOCK_SIZE, rootFileDataBlocks);

		commitMemCopyToDisk(INODE);
		commitMemCopyToDisk(ROOTFILE);
		//Not necessary since currently it is configured to automatically commit ROOTFILE along with INODE 
	}
	else
	{
		createDiskFile(DISK_NO_FORMAT);
	}
	
}


/*
	Copies the contents of rootfile to a unix file with given name
*/
int dumpRootFile(const char* filename)
{
	return copyBlocksToFile(ROOTFILE, ROOTFILE + NO_OF_ROOTFILE_BLOCKS - 1, strdup(filename));
}


/*
	Copies the contents of inode table to a unix file with given name
*/
int dumpInodeTable(const char* filename)
{
	return copyBlocksToFile(INODE, INODE + NO_OF_INODE_BLOCKS - 1, strdup(filename));
}

/*
	This function export the contents of a xfs-file with given name, to a unix file whose path is specifile.
*/
void exportFile(char *filename, char *unixfile)
{
	diskCheckFileExists();

	int i,j,k,l,flag=0,locationOfInode;
	int blk[INODE_NUM_DATA_BLOCKS];
	FILE *outFile;

	for(i=0;i<INODE_NUM_DATA_BLOCKS;i++)
		blk[i] = 0;
	
	locationOfInode = getInodeEntry(filename);
	if(locationOfInode < 0){
		printf("File \'%s\' not found!\n",filename);
		return;
	}
		
	getDataBlocks(blk,locationOfInode);

	expandpath(unixfile);
	outFile = fopen(unixfile,"w");
	if(outFile == NULL)
	{
		printf("File \'%s\' not found!\n", unixfile);
	}
	else
	{
		k = 0;
		while (blk[k] > 0)
		{
			emptyBlock(TEMP_BLOCK);
			readFromDisk(TEMP_BLOCK,blk[k]);
			for(l=0;l<BLOCK_SIZE;l++)
			{
				if(strcmp(disk[TEMP_BLOCK].word[l],"\0")!=0)
					fprintf(outFile,"%s",disk[TEMP_BLOCK].word[l]);
			}
			//printf("next block\n");
			emptyBlock(TEMP_BLOCK);
			k++;
		}
		fclose(outFile);
	}
}

// To expand environment variables in path
void expandpath(char *path) 		
{
	char *rem_path = strdup(path);
	char *token = strsep(&rem_path, "/");
	if(rem_path!=NULL)
		sprintf(path,"%s/%s",getenv(++token)!=NULL?getenv(token):token-1,rem_path);
	else
		sprintf(path,"%s",getenv(++token)!=NULL?getenv(token):token-1);
}

void addext(char *filename, char *ext)
{
	int l = strlen(filename);
	if(l>=16)
	{
		strcpy(filename+11,ext);
		return;
	}
	if(strcmp(filename+l-4,ext)!=0)
	{
		strcat(filename,ext);
		l = strlen(filename);

		if(l>=16)
		{
			strcpy(filename+11,ext);
			return;
		}
	}
}
