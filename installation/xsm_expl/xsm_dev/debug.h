#ifndef _XSM_DEBUG_H

#define _XSM_DEBUG_H

#include "machine.h"
#include "memory.h"

#define OFF FALSE
#define ON TRUE

#define DEBUG_COMMAND_LEN 100
#define DEBUG_STRING_LEN 100

#define DEBUG_STEP			0
#define DEBUG_CONTINUE		1
#define DEBUG_REG			2
#define DEBUG_MEM			3
#define DEBUG_PCB			4
#define DEBUG_PAGETABLE		5
#define DEBUG_FILETABLE		6
#define DEBUG_MEMFREELIST	7
#define DEBUG_DISKFREELIST	8
#define DEBUG_INODETABLE	9
#define DEBUG_USERTABLE		10
#define DEBUG_LOCATION		11
#define DEBUG_WATCH 		12
#define DEBUG_WATCHCLEAR	13
#define DEBUG_EXIT			14
#define DEBUG_HELP			15
#define DEBUG_VAL			16

/* The following is highly dependent on the OS implementation. */
#define DEBUG_LOC_PT		28672
#define MAX_PROC_NUM		16
#define PT_ENTRY_SIZE		16
#define DEBUG_PROC_RUNNING	1 
#define MAX_NUM_PAGES		10
#define PTBR_PCB_OFFSET		14
#define DEBUG_PT_BASE		29696

/* System wide open file table. */
#define DEBUG_LOC_SWOFT		28928
#define MAX_OPENFILE_NUM	32
#define MAX_MEM_PAGE		128
#define DEBUG_LOC_MFL		29184
#define DEBUG_LOC_DFL		31232
#define DISK_SIZE			512
#define DEBUG_LOC_INODE		30208
#define MAX_FILE_NUM 		60
#define DEBUG_LOC_USERTABLE 31168
#define MAX_USER_NUM		16

/* The maximum number of watchpoints permitted. */
#define DEBUG_MAX_WP		16
#define DEBUG_ERROR			-1

struct
_xsm_cpu;

typedef
struct _xsm_cpu
xsm_cpu;

/* The machine that is debugged. */

typedef
struct _debug_status
{
	int state;
	int ip;
	int wp[16];
	int wp_size;
}
debug_status;

/* Function prototypes. */

void
debug_activate ();

void
debug_deactivate ();

int
debug_init ();

int
debug_watch_add (int loc);

void
debug_watch_clear ();

int
debug_watch_test (int mem_min, int mem_max);

int
debug_next_step (int curr_ip);

int
debug_show_interface ();

int
debug_command(char *command);

int
debug_command_code (const char *cmd);

int
debug_display_all_registers();

int
debug_display_register (const char *regname);

int
debug_display_range_reg (const char *reg_b_name, const char *reg_e_name);

int
debug_display_val(char* mem);

int
debug_display_mem(int page);

int
debug_display_mem_range (int page_l, int page_h);

int
debug_display_pcb_pid (int pid);

int
debug_display_fields (int baseptr, const char **fields, const int *fields_len, int n_fields);

int
debug_display_pcb ();

int
debug_active_process ();

int
debug_pcb_base (int pid);

int
debug_display_pt_ptbr ();

int
debug_display_pt_at (int addr);

int
debug_display_pt_pid (int pid);

int
debug_display_ft ();

int
debug_display_memlist();

int
debug_display_dfl();

int
debug_display_inodetable ();

int
debug_display_usertable();

int
debug_display_location (int loc);

#endif
