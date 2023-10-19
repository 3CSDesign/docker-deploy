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

static int running = 0;
static char *pid_file_name = DD_DEFAULT_PID;
static char *log_file_name = DD_DEFAULT_LOG;

static int pid_fd = -1;
static int counter = 0;
static FILE *log_stream;
static int conf_count;
static char* username;



static bool skip_main_proc = true;
static bool daemon_mode = false;
static bool apply_mode = false;
static bool conf_given = false;
static bool test_mode = false;

#include "proc.c"


int main (int argc, char **argv){
  struct options args_info;
  
  if (options(argc, argv, &args_info) != 0)
      exit(1);

  //  username = malloc(strlen(getlogin()));
  //strcpy(username, getlogin());
  /*
  conf_count = count_config(DD_DEFAULT_CONF);
  DeployConfig deployConfig[conf_count];
  read_config(DD_DEFAULT_CONF, &deployConfig);

  int i;
   for (i = 0; i < conf_count; i++){
     DeployConfig conf = deployConfig[i];
   }
   */

   //printf("Username ->:%s\n", getlogin() );

   pid_file_name = "/mnt/part2/code/3cs/docker-deploy/deploy.pid";
   log_file_name = "/mnt/part2/code/3cs/docker-deploy/debug.log";
      
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
     apply();
   else {
     printf("Somethings' not right!");
   }
   
}
