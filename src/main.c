/**
   Janith Cooray
 **/

#include "stdlib.h"
#include "options.c"

int main (int argc, char **argv){
  options_arg_type args_info;
  
  if (options(argc, argv, &args_info) != 0)
    exit(1);

  
}
