#ifndef XSM_TYPES_H

#include "constants.h"

#define XSM_TYPES_H

/* Data types. */
#define XSM_TYPE_STRING 0
#define XSM_TYPE_INTEGER 1

/* Fundamental unit. */
typedef
struct _xsm_word
{
   char val[XSM_WORD_SIZE];
}
xsm_word;

#endif


