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

typedef struct {
  // Name of the project which is also the user 
    char *name;
  // path to the main docker compose file
    char *main_file;
  // path to the aux docker compose file
    char *aux_file;
  // bash script to run during the main startup
    char *on_main_run;
  // bash to check if the main containers are online
    char *on_main_healthcheck;
  // bash script to exec on fail of main
    char *on_main_fail;

    char *on_aux_run;
    char *on_aux_healthcheck;
    char *on_aux_fail;

} DeployConfig;

struct DeployConfig deployConfigs[n];

void read_config(const char* configFile, DeployConfig *config) {
  // Using https://hyperrealm.github.io/libconfig
  config_t cfg;
  config_init(&cfg);
  
  // Read fike
   if (!config_read_file(&cfg, configFile)) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
        config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
   }
  
  const config_setting_t* root = config_root_setting(&cfg);

  const config_setting_t* projects = config_lookup(&cfg, "project");

  
  /*
  if (project != NULL) {
    const char* name;
    config_setting_lookup_string(project, "name", &name);
    config->name = malloc(strlen(name)+1);
    strcpy(config->name,name);
    }*/
   config_destroy(&cfg);
}
