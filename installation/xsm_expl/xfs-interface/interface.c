#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/* For command completion. */
#include <readline/readline.h>
#include <readline/history.h>
#include "interface.h"
#include "fileSystem.h"
#include "exception.h"
#include "diskUtility.h"


jmp_buf exp_point;//for exception handling

/*
 * Strip unwanted characters.
 */
char*
xfs_cli_stripwhite (char *str)
{
	char *end;

	end = str + strlen(str);
	end--;

	while (isspace(*str))
		str++;

	while (isspace(*end) && str <= end)
		end--;

	*++end = '\0';

	return str;
}

/*
 * Set up the auto-complete mechanism.
 */
 void
 cli_init_completion ()
 {
 	rl_readline_name = "xfs-interface";
 	rl_attempted_completion_function = xfs_cli_completion;
 }

/*
 * The command line interface with command completion.
 * Copyright (c) Kurian Jacob et al.
 */
void
cli_run_with_completion ()
{
	char *line_read = NULL, *command;
	while (1)
	{
		/* Return the memory to the pool if needed. */
		if (line_read)
		{
			free (line_read);
			line_read = NULL;
		}

		line_read = readline("# ");

		if (!line_read)
			continue;

		command = xfs_cli_stripwhite (line_read);

		/* Only whitespaces? */
		if (!strlen(command))
			continue;

		add_history(command);

		if (!strcmp(command, "exit"))
			break;

		runCommand(command);
	}

	return;
}

char**
xfs_cli_completion(const char *text, int start, int end)
{
	char **matches;
	char *curr_context;
	char *pch;

	curr_context = malloc(start + 1);
	strncpy (curr_context, rl_line_buffer, start);
	
	/* strncpy is touchy, we have to take care of our frontiers. */
	curr_context[start] = '\0';

	pch = strtok(curr_context, " ");

	matches = NULL;

	rl_completion_append_character = ' ';

	if (pch == NULL)
		matches = rl_completion_matches (text, xfs_cli_command_gen);
	else if (!strcmp(pch, "load"))
	{
		if (rl_line_buffer[start - 1] == '=')
			matches = rl_completion_matches(text, xfs_cli_int_gen);
		else if (!strtok(NULL, " "))
			matches = rl_completion_matches(text, xfs_cli_opt_gen);
	}
	else if (!strcmp(pch, "rm"))
	{
		pch = strtok(NULL, " ");

		if (!pch)
			matches = rl_completion_matches(text, xfs_cli_opt_gen);
		else if (rl_line_buffer[start - 1] == '=')
			matches = rl_completion_matches(text, xfs_cli_int_gen);
		else
			matches = rl_completion_matches(text, xfs_cli_file_gen);
	}
	else if (!strcmp(pch, "cat"))
	{
		matches = rl_completion_matches(text, xfs_cli_file_gen);
	}

	free(curr_context);
	return matches;
}

char*
xfs_cli_command_gen (const char *text, int state)
{
	const char *commands[8]={"fdisk", "load", "rm", "ls", "cat", "copy", "exit", "help"};
	static int index, len;
	const int comm_len = 8;

	if (state == 0)
	{
		index = 0;
		len = strlen(text);
	}

	for (; index < comm_len; ++index)
		if (!strncmp(text, commands[index], len))
			return strdup(commands[index++]);

	return NULL;
}

char*
xfs_cli_opt_gen(const char *text, int state)
{
	const char *options[12] = {"--exec", "--int=", "--exhandler", "--os", "--data"};
	static int index, len;
	const int opt_len = 5;

	if (state == 0)
	{
		index = 0;
		len = strlen(text);
	}

	for (; index < opt_len; ++index)
	{
		if (!strncmp(text, options[index], len))
		{
			/* A bit of hacking, prevent readline from appending a space after possible --int=. */
			if (index == 1)
				rl_completion_append_character = '\0';
			return strdup(options[index++]);
		}
	}

	return NULL;
}

char*
xfs_cli_int_gen (const char *text, int state)
{
	const char *ints[8]={"1", "2", "3", "4", "5", "6", "7", "timer"};
	static int index, len;
	const int ints_len = 8;

	if (state == 0)
	{
		index = 0;
		len = strlen(text);
	}

	for (; index < ints_len; ++index)
		if (!strncmp(text, ints[index], len))
			return strdup(ints[index++]);

	return NULL;
}

static
void
xfs_cli_destroy_file_list(XOSFILE *files)
{
	XOSFILE *curr_ptr, *next_ptr;

	curr_ptr = files;

	while(curr_ptr)
	{
		free(curr_ptr->name);
		next_ptr = curr_ptr->next;

		free(curr_ptr);
		curr_ptr = next_ptr;
	}
}

char*
xfs_cli_file_gen (const char *text, int state)
{
	static int i, j, len;
	static XOSFILE *files;
	char *result = NULL;
	XOSFILE *next;

	if (state == 0)
	{
		xfs_cli_destroy_file_list (files);
		files = getAllFiles();
		len = strlen(text);
	}

	while (files)
	{
		if (!strncmp(text, files->name, len))
		{
			result = strdup(files->name);
		}

		next = files->next;
		free(files->name);
		free(files);
		files = next;

		if (result)
			break;
	}

	return result;
}

/* 
Function to invoke Command Line interface 
*/
void cli(int argc, char **argv)
{
	char command[100], c;
	int i,j;
	if(argc>1)
	{
		strcpy(command,argv[1]);
		i=2;
		while(i<argc)
		{
		
			sprintf(command,"%s %s", command, argv[i]);
			i++;
		}		
		runCommand(command);	
	}
	else
	{
		printf("Unix-XFS Interace Version 2.0. \nType \"help\" for getting a list of commands.\n");
		cli_init_completion();
		cli_run_with_completion();
	}
}

/* 
Function to process commands 
*/
void runCommand(char command[])
{
	char *name = strtok(command, " ");
	char *arg1, *arg2, *arg3;
	
	char * line = NULL;
	int exp_occured;
	exp_occured = setjmp(exp_point);
	if(exp_occured){
		exception_printErrorMessage(exp_occured);	
		return;
	}
	
	if(strcmp(name,"help")==0)		//"help" to display all commands
	{
		printf(" fdisk \n\t Format the disk with XFS filesystem\n");
		printf(" load --exec <pathname> \n\t Loads an executable file to XFS disk \n");
		printf(" load --data <pathname> \n\t Loads a data file to XFS disk \n");
		printf(" load --init <pathname> \n\t Loads INIT code to XFS disk \n");
		printf(" load --os <pathname> \n\t Loads OS startup code to XFS disk \n");
		printf(" load --idle <pathname> \n\t Loads Idle code to XFS disk \n");
		printf(" load --shell <pathname> \n\t Loads Shell code to XFS disk \n");
		printf(" load --library <pathname> \n\t Loads Library code to XFS disk \n");
		printf(" load --int=timer <pathname>\n\t Loads Timer Interrupt routine to XFS disk \n");
		printf(" load --int=disk <pathname>\n\t Loads Disk Controller Interrupt routine to XFS disk \n");
		printf(" load --int=console <pathname>\n\t Loads Console Interrupt routine to XFS disk \n");
		printf(" load --int=[4-18] <pathname>\n\t Loads the specified Interrupt routine to XFS disk \n");
		printf(" load --exhandler <pathname> \n\t Loads exception handler routine to XFS disk \n");
		printf(" load --module [0-7] <pathname>\n\t Loads the specified Module to XFS disk \n");
		printf(" export <xfs_filename> <pathname>\n\t Exports a data file from XFS disk to UNIX file system\n");
		printf(" rm <xfs_filename>\n\t Removes a file from XFS disk \n");
		printf(" ls \n\t List all files\n");
		printf(" df \n\t Display free list and free space\n");
		printf(" cat <xfs_filename> \n\t to display contents of a file\n");
		printf(" copy <start_blocks> <end_block> <unix_filename>\n\t Copies contents of specified range of blocks to a UNIX file.\n");
		printf(" dump --inodetable \n\t Copies the contents of inode table to an external UNIX file named inodetable.txt\n");
		printf(" dump --rootfile \n\t Copies the contents of root file to an external UNIX file named inodetable.txt\n");
		printf(" exit \n\t Exit the interface\n");
	}
	
	
	else if (strcmp(name,"fdisk")==0)	//formatting the disk with XFS partition
	{
		printf("Formatting Complete. \"disk.xfs\" created.\n");
		formatDisk(FORMAT);		
	}
	
	else if (strcmp(name, "run") == 0)	//batch process commands from file
	{
		arg1 = strtok(NULL, " ");
		FILE * fp = fopen(arg1, "r");
		size_t len = 0;
		if (fp == NULL) {
			printf("Unable to open file : %s\n", arg1);
		}
		else {
			while (getline(&line, &len, fp) != -1) {
				if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
				runCommand(line);
				//free(line);
			}
			fclose(fp);
		}
	}
	else if (strcmp(name,"load")==0) 	//loads files to XFS disk.
	{
		arg1 = strtok(NULL, " ");
		arg2 = strtok(NULL, " ");	
		arg3 = strtok(NULL, " ");	

		char *int_command = strtok(arg1, "=");	
		char *intType = strtok(NULL, "=");
		char *fileName = (char*)malloc(51*sizeof(char));
		strncpy(fileName,arg2,50);
		
		if(fileName!=NULL)
			fileName[50] = '\0';
		else
		{
			printf("Missing <pathname> for load. See \"help\" for more information\n");
			return;
		}		
				
		if (strcmp(arg1,"--exec")==0)	
		{
			char *c;
			if (strlen(basename(fileName)) > 12)
			{
				printf("Filename is more than 12 characters long\n");
				return;
			}
			
			c = strrchr(fileName,'.');
			if (c == NULL || strcmp(c,".xsm") != 0)
			{
				printf("Filename does not have \".xsm\" extension\n");
				return;
			}
			
			loadExecutableToDisk(fileName);	 //loads executable file to disk.
		}	
			
		else if (strcmp(arg1,"--init")==0)	
			loadINITCode(fileName);			 //loads init code to disk
		else if (strcmp(arg1,"--shell")==0)	
			loadShellCode(fileName);			 //loads init code to disk
		else if (strcmp(arg1,"--library")==0)	
			loadLibraryCode(fileName);			 //loads init code to disk
		else if (strcmp(arg1,"--idle")==0)	
			loadIdleCode(fileName);			 //loads init code to disk
		else if (strcmp(arg1,"--data")==0) 
		{
			char *c;
			if (strlen(basename(fileName)) > 12)
			{
				printf("Filename is more than 12 characters long\n");
				return;
			}
			
			c = strrchr(fileName,'.');
			if (c == NULL || strcmp(c,".dat") != 0)
			{
				printf("Filename does not have \".dat\" extension\n");
				return;
			}
			
			loadDataToDisk(fileName);		 //loads data file to disk.
		}
		else if (strcmp(arg1,"--os")==0)
			loadOSCode(fileName);			//loads OS startup code to disk
		else if (strcmp(arg1,"--int")==0)
		{
			if(strcmp(intType,"timer")==0)
			{
				loadTimerCode(fileName);
			}
			else if(strcmp(intType,"disk")==0)
			{
				loadDiskControllerIntCode(fileName);
			}
			else if(strcmp(intType,"console")==0)
			{
				loadConsoleIntCode(fileName);
			}
			else
			{
				int intNo = atoi(intType);
				if(intNo >=1 && intNo <=NO_OF_INTERRUPTS)
					loadIntCode(fileName, intNo);
				else
				{
					printf("Invalid argument for \"--int=\" \n");
					return;
				}
			}
		}
		else if (strcmp(arg1,"--module")==0)
		{
			int intNo = atoi(arg2);
			if(intNo >=0 && intNo <=NO_OF_MODULES)
				loadModuleCode(arg3, intNo);
			else
			{
				printf("Invalid argument for \"--module=\" \n");
				return;
			}
		}
		else if (strcmp(arg1,"--exhandler")==0) 
 			{
				loadExHandlerToDisk(fileName);		 //loads exception handler routine to disk.
			}
		else
			printf("Invalid argument \"%s\" for load. See \"help\" for more information\n",arg1);
		free(fileName);
	}	
	
	else if (strcmp(name,"rm")==0) 	//removes files to XFS disk.
	{
		arg1 = strtok(NULL, " ");
		char *fileName = arg1;
		if(fileName!=NULL)
			deleteFileFromDisk(arg1);
		else
			printf("Missing <xfs_filename> for rm. See \"help\" for more information\n");
	}	
	
	else if (strcmp(name,"export")==0) 	//removes files to XFS disk.
	{
		arg1 = strtok(NULL, " ");
		arg2 = strtok(NULL, " ");
		exportFile(arg1,arg2);
	}	

	else if (strcmp(name,"ls")==0)		//Lists all files.
		listAllFiles();
		
	else if (strcmp(name,"df")==0)		//Lists disk free list
		displayDiskFreeList();
				
	else if (strcmp(name,"cat")==0)		//Displays contents of a file
	{
		arg1 = strtok(NULL, " ");
		char *fileName = arg1;
		if(fileName!=NULL)
		{
			//fileName[WORD_SIZE+1] = '\n';
			
			displayFileContents(fileName);
		}
		else
		{
			printf("Missing <xfs_filename> for cat. See \"help\" for more information\n");
			return;
		}	
	}

	else if (strcmp(name,"copy")==0)		//Copies blocks from Disk to UNIX file.
	{
		arg1 = strtok(NULL, " ");
		arg2 = strtok(NULL, " ");
		arg3 = strtok(NULL, " ");
		if(arg1==NULL || arg2==NULL|| arg3==NULL)
		{
			printf("Insufficient arguments for \"copy\". See \"help\" for more information\n");
			return;
		}	
		else
		{
			int startBlock = atoi(arg1);
			int endBlock = atoi(arg2);	
			char *fileName = arg3;			
			fileName[50] = '\0';
			copyBlocksToFile (startBlock,endBlock,fileName);
		}	
	}

	else if (strcmp(name,"dump")==0) 	//loads files to XFS disk.
	{
		arg1 = strtok(NULL, " ");
		if (strcmp(arg1,"--inodetable")==0)
		{
			dumpInodeTable("inodetable.txt");
		}
		else if(strcmp(arg1,"--rootfile")==0)
		{
			dumpRootFile("rootfile.txt");
		}
		else
			printf("Invalid argument \"%s\" for dump. See \"help\" for more information\n",arg1);
	}

	else if (strcmp(name,"exit")==0)		//Exits the interface
		exit(0);
	else
		printf("Unknown command \"%s\". See \"help\" for more information\n",name);
}



int main(int argc, char **argv){
	int intNo, fd;

	disk_init();

	fd = open(DISK_NAME, O_RDONLY, 0666);
	if(fd > 0)
	{
		close(fd);
		loadFileToVirtualDisk();
	}

	cli(argc, argv);					//Loads the Command Line Interface
	return 0;
}
