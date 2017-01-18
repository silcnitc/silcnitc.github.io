#ifndef LABELS_H
#define LABELS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"

typedef struct _label{
	char *name;
	int address;
	struct _label *next;
}label;

void labels_reset ();
int labels_phase_one(FILE *fp);
int labels_is_label (const char *str);
char* labels_get_name (char *label);
void labels_insert (char *label, int address);
int labels_phase_two (FILE *fin, FILE *fout, int base_address);
int labels_get_target (const char *name);
int labels_is_charstring (char *str);
int labels_resolve (const char *filename, char *outfile, int base_address);
void labels_random_name (char *name);

#endif
