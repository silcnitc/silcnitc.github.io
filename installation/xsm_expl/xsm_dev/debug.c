#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static
debug_status
_db_status;

const
char *_db_commands_lh[] = {
	"step",
	"continue",
	"reg",
	"mem",
	"pcb",
	"pagetable",
	"filetable",
	"memfreelist",
	"diskfreelist",
	"inodetable",
	"usertable",
	"location",
	"watch",
	"watchclear",
	"exit",
	"help",
	"val"
};

const
char *_db_commands_sh[] = {
	"s",
	"c",
	"r",
	"m",
	"p",
	"pt",
	"ft",
	"mf",
	"df",
	"it",
	"ut",
	"l",
	"w",
	"wc",
	"e",
	"h",
	"v"
};

int
debug_init ()
{
	_db_status.state = OFF;
	
	debug_watch_clear ();

	return TRUE;
}

int
debug_watch_add (int loc)
{
	if (_db_status.wp_size >= DEBUG_MAX_WP)
		return FALSE;

	_db_status.wp[_db_status.wp_size] = loc;
	_db_status.wp_size++;
	return TRUE;
}

void
debug_watch_clear ()
{
	_db_status.wp_size = 0;
}

int
debug_watch_test (int mem_min, int mem_max)
{
	int i;

	if (mem_min < 0)
		return DEBUG_ERROR;

	for (i = 0; i < _db_status.wp_size; ++i)
	{
		if (mem_min <= _db_status.wp[i] && _db_status.wp[i] <= mem_max)
		{
			return i;
		}
	}

	return DEBUG_ERROR;
}

/* The instruction fetch-exec cycle will repeatedly call this function before each
 * instruction exec phase.
 * This function will invoke the debug mode, if necessary.
 * @param curr_ip The current [physical] address of IP.
 */
int
debug_next_step (int curr_ip)
{
	int mem_low, mem_high;
	int wp = DEBUG_ERROR;

	_db_status.ip = curr_ip;

	machine_get_mem_access (&mem_low, &mem_high);

	if (mem_low > 0)
		wp = debug_watch_test(mem_low, mem_high);

	if (wp > 0)
	{
		printf ("Watchpoint at %d has triggered the debugger.\n", _db_status.wp[wp]);
		_db_status.state = ON;
	}

	if (_db_status.state == ON)
	{
		return debug_show_interface ();
	}

	return TRUE;
}

int
debug_show_interface ()
{
	char command[DEBUG_COMMAND_LEN];
	int done = FALSE;
	char next_instr[DEBUG_STRING_LEN];

	memory_retrieve_raw_instr (next_instr, machine_translate_address(_db_status.ip,FALSE));

	printf ("Next instruction to execute: %s\n", next_instr);

	while (!done)
	{
		fgets (command, DEBUG_COMMAND_LEN, stdin);

		// remove the dangling \n from fgets
		strtok(command, "\n");

		if (!strcmp(command, "exit") || !strcmp(command, "e")){
			debug_deactivate();
			return FALSE;
		}
		
		done = debug_command (command);
	}

	return TRUE;
}

int
debug_command(char *command)
{
	char *arg1, *cmd, *arg2;
	int code;

	const char *delim = " \t";

	cmd = strtok(command, delim);

	code = debug_command_code (cmd);

	switch (code)
	{
		case DEBUG_STEP:
			return TRUE;

		case DEBUG_CONTINUE:
			debug_deactivate();
			return TRUE;

		case DEBUG_REG:
			arg1 = strtok(NULL, delim);
			if (!arg1)
			{
				debug_display_all_registers();
			}
			else{
				arg2 = strtok(NULL, delim);
				if (!arg2)
				{
					debug_display_register(arg1);
				}
				else
				{
					debug_display_range_reg(arg1, arg2);
				}
			}
			break;

		case DEBUG_MEM:
			arg1 = strtok (NULL, delim);
			arg2 = strtok (NULL, delim);

			if (arg2)
			{
				debug_display_mem_range(atoi(arg1), atoi(arg2));
			}
			else
			{
				debug_display_mem(atoi(arg1));
			}
			break;

		case DEBUG_PCB:
			arg1 = strtok (NULL, delim);

			if (!arg1)
				debug_display_pcb();
			else
				debug_display_pcb_pid (atoi(arg1));
			break;

		case DEBUG_PAGETABLE:
			arg1 = strtok (NULL, delim);

			if (!arg1)
			{
				debug_display_pt_ptbr();
			}
			else
			{
				debug_display_pt_pid(atoi(arg1));
			}
			break;

		case DEBUG_FILETABLE:
			debug_display_ft();
			break;

		case DEBUG_MEMFREELIST:
			debug_display_memlist();
			break;
			
		case DEBUG_DISKFREELIST:
			debug_display_dfl();
			break;

		case DEBUG_INODETABLE:
			debug_display_inodetable();
			break;	
			
		case DEBUG_VAL:
			arg1 = strtok (NULL, delim);
			debug_display_val (arg1);
			
		break;
		
		default:
			printf("Unknown command \"%s\". See \"help\" for more information.\n",command);
	}

	return FALSE;
}

int
debug_command_code (const char *cmd)
{
	int i;

	for (i = 0; i <= DEBUG_VAL; ++i)
	{
		if (!strcmp(cmd, _db_commands_lh[i]))
			return i;
	}

	for (i = 0; i <= DEBUG_VAL; ++i)
	{
		if (!strcmp(cmd, _db_commands_sh[i]))
			return i;
	}

	return -1;
}

int
debug_display_all_registers()
{
	const char **reg_names = registers_names ();
	int num_regs = registers_len ();
	int i;
	char *content;

	for (i = 0; i < num_regs; ++i)
	{
		content = registers_get_string (reg_names[i]);
		printf ("%s: %s\n", reg_names[i], content);
	}

	return TRUE;
}

int
debug_display_register (const char *regname)
{
	char *content;

	content = registers_get_string (regname);

	if (!content)
	{
		printf ("No such register.\n");
		return FALSE;
	}

	printf ("%s: %s\n", regname, content);
	return TRUE;
}

int
debug_display_range_reg (const char *reg_b_name, const char *reg_e_name)
{
	const char **reg_names = registers_names ();
	int num_regs = registers_len ();
	int i;
	char *content;

	for (i = 0; i <  num_regs; ++i)
	{
		if (!strcmp (reg_b_name, reg_names[i]))
			break;
	}

	for (; i < num_regs; ++i)
	{
		content = registers_get_string (reg_names[i]);

		printf ("%s: %s\n", reg_names[i], content);

		if (!strcmp (reg_e_name, reg_names[i]))
			break;
	}

	return TRUE;
}

int
debug_display_mem(int page)
{
	xsm_word *word;
	int i, ptr;
	char *content;

	FILE *fp;
	
	fp = fopen("mem","w");
	
	page = machine_translate_address(page*512,0) / 512;
	
	word = memory_get_page(page);

	if (!word)
	{
		printf ("No such page.\n");
		return FALSE;
	}

	ptr = page * XSM_PAGE_SIZE;

	// write to file mem
	
	for (i = 0; i < XSM_PAGE_SIZE; i++)
	{
		word = memory_get_word(ptr);
		content = word_get_string(word);
		fprintf(fp,"+%d: %s\n", i, content);
		ptr++; 
	}
	
	fclose(fp);
	
	printf("Written to file mem\n");
	return TRUE;
}


int
debug_display_mem_range (int page_l, int page_h)
{
	int i;

	for (i = page_l; i <= page_h; ++i)
	{
		printf ("Page: %d\n", i);
		debug_display_mem(i);
	}

	return TRUE;
}

int debug_display_val(char *mem){
	xsm_word *mword;	
	int addr = machine_translate_address(atoi(mem),0);
	printf("AD : %d\n",addr);
	mword = memory_get_word(addr);
	printf("%s\n",word_get_string(mword));
	
	return TRUE;
	}
	
int
debug_display_pcb_pid (int pid)
{	
	const char *fields[] = {"Tick", "PID", "PPID", "UserID", "State", "Swap Flag", "Inode Index",
	"Input Buffer", "Mode Flag", "User Area Swap Status", "User Area Page Number",
	"Kernel Stack Pointer", "User Stack Pointer", "PTBR", "PTLR", "Unused"
	};
	
	const int fields_len[] = {1, 1, 1, 1 , 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	const int n_fields = 15;
	int ptr;

	ptr = DEBUG_LOC_PT + pid * PT_ENTRY_SIZE;

	printf("DEB %d \n",DEBUG_LOC_PT);
	printf("DEB %d \n",pid);
	printf("DEB %d \n",PT_ENTRY_SIZE);
	
	return debug_display_fields(ptr, fields, fields_len, n_fields);
}

int
debug_display_fields (int baseptr, const char **fields, const int *fields_len, int n_fields)
{
	int i, ptr;
	xsm_word *word;

	ptr = baseptr;
	
	for (i = 0; i < n_fields; ++i)
	{
		printf ("%s:		", fields[i]);

		/* Display the corresponding number of words. */
		int j;

		for (j = 0; j < fields_len[i]; ++j)
		{
			word = memory_get_word(ptr);
			printf ("%s ", word_get_string(word));
			ptr = ptr + 1;

		}

		printf ("\n");
	}

	return TRUE;
}

int
debug_display_pcb ()
{
	int pid;

	pid = debug_active_process();

	if (pid > -1)
	{
		debug_display_pcb_pid(pid);
		return TRUE;
	}
	
	printf ("No active processes.\n");
	return FALSE;
}

/* Returns the PID of the active process.*/
int
debug_active_process ()
{
	int ptr, pid, i;
	int pid_base, state_base;
	xsm_word *w_pid_base, *w_state_base;

	ptr = DEBUG_LOC_PT;
	pid = -1;

	/* Determine the active process and display it. */
	for (i = 0; i < MAX_PROC_NUM; ++i)
	{
		pid_base = ptr + 1;
		state_base = ptr + 4;

		w_pid_base = memory_get_word (pid_base);
		w_state_base = memory_get_word(state_base);

		if (DEBUG_PROC_RUNNING == word_get_integer(w_state_base))
		{
			pid = word_get_integer(w_pid_base);
			break;
		}

		ptr = ptr + PT_ENTRY_SIZE;
	}

	return pid;
}

int
debug_pcb_base (int pid)
{
	int ptr, pid_base, i;
	xsm_word *w_pid_base;
	int result = -1;

	ptr = DEBUG_LOC_PT;

	for (i = 0; i < MAX_PROC_NUM; ++i)
	{
		pid_base = ptr + 1;

		w_pid_base = memory_get_word(pid_base);

		if (pid == word_get_integer(w_pid_base))
		{
			result = ptr;
			break;
		}

		ptr = ptr + PT_ENTRY_SIZE;
	}

	return result;
}

int
debug_display_pt_ptbr ()
{
	int addr;

	addr = registers_get_integer("PTBR");

	return debug_display_pt_at (addr);
}

int
debug_display_pt_at (int addr)
{
		
	int i, ptr;
	xsm_word *word;

	ptr = addr;

	for (i = 0; i < MAX_NUM_PAGES; ++i)
	{
		printf ("VIRT: %d\t\t", i);
		
		word = memory_get_word(ptr);
		printf ("PHY: %s\t\t", word_get_string(word));
	
		ptr = ptr + 1;
		
		word = memory_get_word(ptr);
		printf ("AUX: %s\t\n", word_get_string(word));
		
		ptr = ptr + 1;
	}

	return TRUE;
}

int
debug_display_pt_pid (int pid)
{
	
	int ptbr_addr;
	
	ptbr_addr = DEBUG_PT_BASE + pid * MAX_NUM_PAGES * 2; 
	return debug_display_pt_at(ptbr_addr);
}

int
debug_display_ft ()
{
	int ptr, i;
	xsm_word *word;

	ptr = DEBUG_LOC_SWOFT;

	for (i = 0; i < MAX_OPENFILE_NUM; ++i)
	{
		word = memory_get_word(ptr++);
		printf ("Inode Index %s\t", word_get_string(word));

		word = memory_get_word(ptr++);
		printf("Open Instance Count %s\t", word_get_string(word));

		word = memory_get_word(ptr++);
		printf ("Lseek %s\n", word_get_string(word));
		ptr++; /* Unused field. */
	}

	return TRUE;
}

int
debug_display_memlist()
{
	int i, ptr;
	xsm_word *word;

	ptr = DEBUG_LOC_MFL;

	for (i = 0; i < MAX_MEM_PAGE;)
	{
		word = memory_get_word(ptr++);
		printf ("%d\t%s\t\t", i, word_get_string(word));
		
		word = memory_get_word(ptr++);
		printf ("%d\t%s\t\t", i+1, word_get_string(word));
		
		word = memory_get_word(ptr++);
		printf ("%d\t%s\t\t", i+2, word_get_string(word));
		
		word = memory_get_word(ptr++);
		printf ("%d\t%s\n", i+3, word_get_string(word));
		
		i = i + 4;
	}

	return TRUE;
}

int
debug_display_dfl()
{
	int i, ptr;
	xsm_word *word;

	ptr = DEBUG_LOC_DFL;

	for (i = 0; i < DISK_SIZE; ++i)
	{
		word = memory_get_word(ptr++);
		printf("%d\t%s\n", i, word_get_string(word));
	}

	return TRUE;
}

int
debug_display_inodetable ()
{
	const char *fields[] = {
		"Type", "Name", "Size", "UID", "Perm.", "Unused", "D1", "D2", "D3", "D4", "Unused"
	};
	const int fields_len[] = {
		1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 4
	};
	int i, ptr;
	const int n_fields = 11;
	const int entry_size = 16;

	ptr = DEBUG_LOC_INODE;

	for (i = 0; i < MAX_FILE_NUM; ++i)
	{
		debug_display_fields (ptr, fields, fields_len, n_fields);
		printf ("\n");

		/* Size of each entry is 16. */
		ptr = ptr + entry_size;
	}

	return TRUE;
}

void
debug_activate ()
{
	/* Activate the debugger. */
	_db_status.state = ON;
}

void
debug_deactivate ()
{
	/* Activate the debugger. */
	_db_status.state = OFF;
}

int
debug_display_usertable()
{
	const char *fields[] = {
		"User name", "Encrypted Password"
	};
	const int fields_len[] = {
		1, 1
	};
	int i, ptr;
	const int n_fields = 2;
	const int entry_size = 2;

	ptr = DEBUG_LOC_USERTABLE;

	for (i = 0; i < MAX_USER_NUM; ++i)
	{
		debug_display_fields (ptr, fields, fields_len, n_fields);
		printf ("\n");

		/*Update ptr.*/
		ptr = ptr + entry_size;
	}

	return TRUE;
}

int
debug_display_location (int loc)
{
	xsm_word *word;
	int mode, ptbr;

	mode = machine_get_mode();

	if (PRIVILEGE_KERNEL == mode)
	{
		word = memory_get_word (loc);
	}
	else
	{
		int tr_loc;

		ptbr = registers_get_integer("PTBR");
		tr_loc = memory_translate_address (ptbr, loc, FALSE);

		if (tr_loc < 0)
		{
			printf ("A translation for this address is not available.\n");
			return FALSE;
		}

		word = memory_get_word(tr_loc);
	}

	printf ("%s\n", word_get_string(word));
	return TRUE;
}
