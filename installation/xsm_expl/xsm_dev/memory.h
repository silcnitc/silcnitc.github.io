#ifndef _XSM_MEMORY_H

#define _XSM_MEMORY_H

#include "types.h"
#include "word.h"


#define XSM_MEMORY_SIZE (XSM_PAGE_SIZE*XSM_MEMORY_NUMPAGES)
#define XSM_MEM_NOWRITE -1
#define XSM_MEM_PAGEFAULT -2

int
memory_init ();

xsm_word*
memory_get_word (int address);

int
memory_is_address_valid (int address);

int
memory_addr_page(int address);

int
memory_translate_address (int ptbr, int address, int write);

int
memory_translate_page (int ptbr, int page, int write);

void
memory_retrieve_raw_instr(char *dest, int address);

xsm_word*
memory_get_page (int page);

void 
memory_destroy();

#endif
