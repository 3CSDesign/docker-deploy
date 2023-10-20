/*

  Copyright (C) 2023 3CS
  Copyright (C) 2023 Janith Cooray
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at
  your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see https://www.gnu.org/licenses/.

*/

#include "main.h"

static bool skip_main_proc = true;
static bool daemon_mode = false;
int main (int argc, char **argv){
  struct options args_info;
  
  if (options(argc, argv, &args_info) != 0)
      exit(1);

   if (args_info.config_given) {
     load_config(args_info.config_arg);
     
     set_pid();
     set_log();
     printf("Looking into PID - %s\n",pid_file_name);
     
     d_open_file(log_file_name);
     d_open_file(pid_file_name);

     if (args_info.test_given) {
       return test_config();
     }

     if (args_info.apply_given) {
       return apply();
     }

     if (args_info.daemon_given)
      daemonize();
     return main_proccess();  
   }

   printf("Missing Config!\n");
   exit(EXIT_FAILURE);
  
}
