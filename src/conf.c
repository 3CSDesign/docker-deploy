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

DeployConfig deployConfigs[100];

/**
   Include File Option
 **/
static const char **include_func(config_t *config,
                                 const char *include_dir,
                                 const char *path,
                                 const char **error)
{
  char *p;
  DIR *dp;
  struct dirent *dir_entry;
  struct stat stat_buf;
  char include_path[PATH_MAX + 1];
  size_t include_path_len = 0;
  char file_path[PATH_MAX + 1];
  char **result = NULL;
  char **result_next = result;
  int result_count = 0;
  int result_capacity = 0;

  *include_path = 0;

  if(*path != '/')
  {
    if(include_dir)
    {
      strcat(include_path, include_dir);
      include_path_len += strlen(include_dir);
    }
  }

  p = strrchr(path, '/');
  if(p > path)
  {
    int len = p - path;

    if((include_path_len > 0)
       && (*(include_path + include_path_len - 1) != '/'))
    {
      strcat(include_path, "/");
      ++include_path_len;
    }

    strncat(include_path, path, len);
    include_path_len += len;
  }

  if(include_path_len == 0)
  {
    strcpy(include_path, ".");
    include_path_len = 1;
  }

  dp = opendir(include_path);
  if(dp)
  {
    while((dir_entry = readdir(dp)) != NULL)
    {
      int r = snprintf(file_path, PATH_MAX, "%s/%s", include_path,
                       dir_entry->d_name);
      if(r < 0) abort(); // Handle possible truncation of very long path
      
      if(lstat(file_path, &stat_buf) != 0) continue;
      if(!S_ISREG(stat_buf.st_mode)) continue;
      if(fnmatch(path, file_path, FNM_PATHNAME) != 0) continue;

      if(result_count == result_capacity)
      {
        result_capacity += 16;
        result = (char **)realloc(result, (result_capacity + 1) * sizeof(char *));
        result_next = result + result_count;
      }

      *result_next = strdup(file_path);
      ++result_next;
      ++result_count;
    }
    closedir(dp);
  }

  *result_next = NULL;

  return((const char **)result);
}

void read_config(const char* configFile) {
  // Using https://hyperrealm.github.io/libconfig
  config_t cfg;
  config_init(&cfg);
  config_set_include_func(&cfg, include_func);  
  // Read fike
   if (!config_read_file(&cfg, configFile)) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
        config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
   }
  
  const config_setting_t* projects = config_lookup(&cfg,"deploy.projects");

  if (projects != NULL){
    int count = config_setting_length(projects);
    int i;
    for(i = 0; i < count; ++i)
      {
	config_setting_t *project = config_setting_get_elem(projects, i);
	const char* name,
	  main_file,
	  aux_file,
	  on_main_run,
	  on_main_healthcheck,
	  on_main_fail,
	  on_aux_run,
	  on_aux_healthcheck,
	  on_aux_fail;

	if(!(config_setting_lookup_string(project, "name", &name)))
	  continue;

	config_setting_lookup_string(project, "name", &name);
	config_setting_lookup_string(project, "main_file", &main_file);/*
	config_setting_lookup_string(project, "aux_file", &aux_file);/*
	config_setting_lookup_string(project, "on_main_run", &on_main_run);
	config_setting_lookup_string(project, "on_main_healthcheck", &on_main_healthcheck);
	config_setting_lookup_string(project, "on_main_fail", &on_main_fail);
	config_setting_lookup_string(project, "on_aux_run", &on_aux_run);
	config_setting_lookup_string(project, "on_aux_healthcheck", &on_aux_healthcheck);
	config_setting_lookup_string(project, "on_aux_fail", &on_aux_fail);*/


	deployConfigs[i].name = malloc(strlen(name)+1);/*
	deployConfigs[i].main_file = malloc(strlen(main_file)+1);
	deployConfigs[i].aux_file = malloc(strlen(aux_file)+1);/*
	deployConfigs[i].on_main_run = malloc(strlen(on_main_run)+1);
	deployConfigs[i].on_main_healthcheck = malloc(strlen(on_main_healthcheck)+1);
	deployConfigs[i].on_main_fail = malloc(strlen(on_main_fail)+1);
	deployConfigs[i].on_aux_run = malloc(strlen(on_aux_run)+1);
	deployConfigs[i].on_aux_healthcheck = malloc(strlen(on_aux_healthcheck)+1);
	deployConfigs[i].on_aux_fail = malloc(strlen(on_aux_fail)+1);*/
	
	strcpy(deployConfigs[i].name,name);/*
	strcpy(deployConfigs[i].main_file,main_file);
	strcpy(deployConfigs[i].aux_file,aux_file);
	strcpy(deployConfigs[i].on_main_run,on_main_run);
	strcpy(deployConfigs[i].on_main_healthcheck,on_main_healthcheck);
	strcpy(deployConfigs[i].on_main_fail,on_main_fail);
	strcpy(deployConfigs[i].on_aux_run,on_aux_run);
	strcpy(deployConfigs[i].on_aux_healthcheck,on_aux_healthcheck);
	strcpy(deployConfigs[i].on_aux_fail,on_aux_fail);*/

	printf("Name %s\n", name);
	
      }

  } else {
    printf("No project detected!\n");
  }
   config_destroy(&cfg);
}
