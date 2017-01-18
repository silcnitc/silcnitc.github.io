#ifndef _XSM_SIMULATOR_H

#define _XSM_SIMULATOR_H
#include "machine.h"

#define XSM_SIMULATOR_DEFDEBUG FALSE
#define XSM_SIMULATOR_DEFCONSOLE 10
#define XSM_SIMULATOR_DEFTIMER 10
#define XSM_SIMULATOR_DEFDISK 10

int
simulator_run ();

int
simulator_parse_args(int argc, char **argv);

#endif