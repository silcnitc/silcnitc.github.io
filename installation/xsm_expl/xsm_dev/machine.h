#ifndef XSM_MACHINE_H

#define XSM_MACHINE_H

#include <setjmp.h>

#include "types.h"
#include "registers.h"
#include "memory.h"
#include "disk.h"
#include "exception.h"
#include "debug.h"

#define XSM_ADDR_DREF 0
#define XSM_ADDR_NODREF 1

#define PRIVILEGE_USER 0
#define PRIVILEGE_KERNEL 1

/* Any random value other than zero. */
#define XSM_EXCEPTION_OCCURED 256

/* Operation codes. */
#define MOV       0
#define ADD       1
#define SUB       2
#define MUL       3
#define DIV       4
#define MOD       5
#define INR       6
#define DCR       7
#define LT        8
#define GT        9
#define EQ        10
#define NE        11
#define GE        12
#define LE        13
#define JZ        14
#define JNZ       15
#define JMP       16
#define PUSH      17
#define POP       18
#define CALL      19
#define RET       20
#define BRKP      21
#define INT       22

#define LOADI     23 
#define LOAD      24
#define STORE     25
#define ENCRYPT   26
#define BACKUP    27
#define RESTORE   28
#define PORT      29
#define IN        30
#define INI       31
#define OUT       32
#define IRET      33
#define HALT      34

/* Between these values are the privileged instructions. */
#define TOKEN_KERN_LOW 23
#define TOKEN_KERN_HIGH 34

#define XSM_INSTRUCTION_COUNT 35

#define XSM_DISKOP_LOAD 0
#define XSM_DISKOP_STORE 1

#define XSM_CONSOLE_PRINT 0
#define XSM_CONSOLE_READ 1

/* -1 is the number of exception handler *interrupt* */
#define XSM_INTERRUPT_EXHANDLER 0

#define XSM_HALT 1654

typedef
struct _disk_operation
{
   int src_block;
   int dest_page;
   int operation;
} disk_operation;

typedef
struct _console_operation
{
   xsm_word word;
   int operation;
} console_operation;

typedef 
struct _xsm_cpu
{
   xsm_reg *regs;
   int timer;
   int mode;
   int disk_state, disk_wait;
   int console_state, console_wait;

   /* For debugging purpose.
    * Whether there was a write to RAM and to which address was it
    * written to. A positive number denotes a valid write.
    * This member is used to manage watch while debugging. 
    * There are two values. A low and a high. This is because the
    * XSM processor supports multiple memory accesses in a single instruction(SIMD).
    */
   int mem_low, mem_high;

   disk_operation disk_op;
   console_operation console_op;

   /* Exception point */
   jmp_buf h_exp_point;
}
xsm_cpu;

typedef
struct _xsm_options
{
   int timer;
   int debug;
   int disk;
   int console;
}
xsm_options;

int
machine_init (xsm_options *options);

int
machine_get_opcode (const char* instr);

xsm_word *
machine_get_ipreg ();

xsm_word *
machine_get_spreg ();

int
machine_run ();

/* The one point for all exception handling. */
int
machine_handle_exception();

void
machine_post_execute ();

int
machine_execute_instruction (int opcode);

int
machine_execute_logical (int opcode);

int
machine_execute_brkp ();

int
machine_execute_unary (int opcode);

int
machine_execute_mov ();

xsm_word*
machine_get_address (int write);

int
machine_get_address_int (int write);

int
machine_translate_address (int address, int write);

xsm_word*
machine_memory_get_word (int address);

int
machine_execute_arith (int opcode);

int
machine_execute_jump (int opcode);

int
machine_execute_stack (int opcode);

int
machine_push_do (xsm_word *reg);

int
machine_pop_do (xsm_word *dest);

xsm_word*
machine_stack_pointer (int write);

int
machine_execute_call_do (int target);

int
machine_execute_backup();

int
machine_execute_restore ();

int
machine_execute_call ();

int
machine_execute_ret ();

int
machine_execute_interrupt();

int 
machine_execute_interrupt_do (int interrupt);

int
machine_interrupt_address (int int_num);

void
machine_set_mode (int mode);

int
machine_execute_disk (int operation, int immediate);

int
machine_execute_store_do (int page_num, int block_num);

int
machine_schedule_disk (int page_num, int block_num, int firetime, int operation);

int
machine_execute_load_do (int page_num, int block_num);

int
machine_execute_encrypt ();

int
machine_execute_print_do (xsm_word *word);

int
machine_execute_print ();

int
machine_schedule_in(int firetime);

int
machine_execute_ini ();

int
machine_execute_in_do (xsm_word *word);

int
machine_execute_iret ();

void
machine_destroy ();

void
machine_raise_exception (const char *exp);

void
machine_pre_execute ();

int
machine_get_mode ();

xsm_word*
machine_get_register (const char *name);

int
machine_instr_req_privilege (int opcode);

void
machine_get_mem_access (int _output_ *mem_low, int _output_ *mem_high);

int
machine_read_disk_arg();

#endif
