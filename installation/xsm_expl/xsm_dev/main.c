#include <stdlib.h>
#include "simulator.h"

int 
main(int argc, char **argv)
{
   if (!simulator_parse_args(argc, argv))
      return EXIT_FAILURE;
      
   simulator_run ();
   
   /*Done.*/
   return EXIT_SUCCESS;   
}
