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

   pid_file_name = "/mnt/part2/code/3cs/docker-deploy/deploy.pid";
   log_file_name = "/mnt/part2/code/3cs/docker-deploy/debug.log";

   /*
   if (args_info.daemon_given) {
     daemon_mode = true;
     skip_main_proc = false;
   }

   if (args_info.help_given)
     skip_main_proc = true;
   if (args_info.test_given)
     test_mode = true;
   if (args_info.version_given)
     skip_main_proc = true;
   if (args_info.apply_given) {
     skip_main_proc = true;
     apply_mode = true;
   }

  
   // If config is provided, Load it anyway
   if(args_info.config_given)
     load_config(args_info.config_arg);

   if (skip_main_proc == false) {
     
     if (conf_given)
       exit(1);
     daemonize();
     return main_proccess();
   }
   
   if(test_mode)
     return test_config();

   if (apply_mode)
     return apply();
   else {
     return main_proccess();
   
     }*/
   
   if (args_info.config_given) {
     load_config(args_info.config_arg);

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
