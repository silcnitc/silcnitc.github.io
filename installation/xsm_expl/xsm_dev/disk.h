#ifndef _XSM_DISK_H

#define _XSM_DISK_H

#include "types.h"

#define XSM_DISK_BLOCK_NUM 512
#define XSM_DISK_BLOCK_SIZE XSM_PAGE_SIZE /* Otherwise not very effective. */

int
disk_init (const char *filename);

int
disk_write_page (xsm_word *page, int block_num);

xsm_word*
disk_get_block (int block);

int
disk_read_block (xsm_word *page, int block_num);

int
disk_close ();

#endif