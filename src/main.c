/*
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

int main (int argc, char **argv){
  options_arg_type args_info;
  
  //if (options(argc, argv, &args_info) != 0)
    //  exit(1);
   DeployConfig config;
   read_config("main.conf", &config);

   // Use the configuration variables as needed
   printf(config.name);
   printf("Main File: %s\n", config.main_file);
   printf("Aux File: %s\n", config.aux_file);
   printf("On Main Run:\n%s\n", config.on_main_run);
  
}
