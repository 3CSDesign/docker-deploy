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

typedef struct {
  /**
     Abosolute path for PID file
     must match systemd pid location
   **/
  char* pid_file_path;
  /**
     Absolute path for Log file
   **/
  char* log_file_path;
} global_config;

typedef struct {
  int count;
  char** run_script;
} runner_structs;

typedef struct {
  // Name of the project which is also the user 
    char *name;
  // path to the main docker compose file
    char *main_file;
  // path to the aux docker compose file
    char *aux_file;
  // bash script to run during the main startup
    runner_structs on_main_run;
  // bash to check if the main containers are online
    runner_structs on_main_healthcheck;
  // bash script to exec on fail of main
  runner_structs on_main_fail;
  runner_structs on_aux_run;
  runner_structs on_aux_healthcheck;
  runner_structs on_aux_fail;

} DeployConfig;
