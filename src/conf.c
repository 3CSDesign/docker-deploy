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

#include "conf.h"

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

void alloc_setting(config_setting_t *config_setting,
			     const char* setting_name ,
			     char* *setting)
{
  const char* tmp;
  config_setting_lookup_string(config_setting, setting_name, &tmp);
  *setting = malloc(strlen(tmp)+1);
  strcpy(*setting,tmp);
}

void alloc_arr(config_setting_t *config_setting,
			     const char* setting_name ,
				 runner_structs *setting)
{
  config_setting_t* tmp = config_setting_lookup( config_setting , setting_name);
  if (tmp != NULL){
    int tmp_count = config_setting_length(tmp);
    int i;
    setting->run_script = malloc(tmp_count * sizeof(char*));
    for(i = 0; i < tmp_count; i++){
      config_setting_t *scripts = config_setting_get_string_elem(tmp, i);
      setting->run_script[i] = malloc(strlen(scripts));
      strcpy(setting->run_script[i], scripts);
    }
  }

}

/**
   @TODO config file is read twice, optimise!
 **/
int count_config(const char* configFile){
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
  int count = config_setting_length(projects);
  config_destroy(&cfg);
  return count;
}

void read_config(const char* configFile, DeployConfig *deployConfigs) {
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

	alloc_setting(project,"name",&deployConfigs[i].name);
	alloc_setting(project,"main_file",&deployConfigs[i].main_file);
	alloc_setting(project,"aux_file",&deployConfigs[i].aux_file);
	
	alloc_arr(project, "on_main_run", &deployConfigs[i].on_main_run);
	alloc_arr(project, "on_main_healthcheck", &deployConfigs[i].on_main_healthcheck);
	alloc_arr(project, "on_main_fail", &deployConfigs[i].on_main_fail);
	alloc_arr(project, "on_aux_run", &deployConfigs[i].on_aux_run);
	alloc_arr(project, "on_aux_healthcheck", &deployConfigs[i].on_aux_healthcheck);
	alloc_arr(project, "on_aux_fail", &deployConfigs[i].on_aux_fail);
	
      }
    config_destroy(&cfg);
 
  } else {
    printf("No project detected!\n");
    exit(1);
  }
}
