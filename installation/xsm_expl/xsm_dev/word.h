#ifndef _XSM_WORD_H

#define _XSM_WORD_H

#include "types.h"

int
word_get_integer (xsm_word* word);

int
word_get_unix_type (xsm_word *word);

char*
word_get_string (xsm_word *word);

int
word_store_integer (xsm_word *word, int integer);

int
word_store_string (xsm_word *word, const char *str);

void
word_copy (xsm_word* dest, xsm_word *src);

void
word_encrypt(xsm_word *word);

#endif
