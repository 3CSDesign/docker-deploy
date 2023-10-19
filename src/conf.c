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

void free_setting(char* *setting){
  free(setting);
}

void alloc_arr(config_setting_t *config_setting,
			     const char* setting_name ,
				 runner_structs *setting)
{
  config_setting_t* tmp = config_setting_lookup( config_setting , setting_name);
  if (tmp != NULL){
    int tmp_count = config_setting_length(tmp);
    int i;
    setting->count = tmp_count;
    setting->run_script = malloc(tmp_count * sizeof(char*));
    for(i = 0; i < tmp_count; i++){
      config_setting_t *scripts = config_setting_get_string_elem(tmp, i);
      setting->run_script[i] = malloc(strlen(scripts));
      strcpy(setting->run_script[i], scripts);
    }
  }

}

void free_arr(runner_structs *setting){
  
  int i;
  for(i = 0; i < setting->count; i ++){
    free(setting->run_script[i]);
  }
  free(setting->run_script);
}

void destroy_deploy_conf(DeployConfig *deployConfig, int count){
    int i;
    for(i = 0; i < count; ++i)
      {
	/**
	   Collect Garbage
	 **/
	free_setting(deployConfig[i].name);
	free_setting(deployConfig[i].main_file);
	free_setting(deployConfig[i].aux_file);

	free_arr(&deployConfig[i].on_main_run);
	free_arr(&deployConfig[i].on_main_healthcheck);
	free_arr(&deployConfig[i].on_main_fail);
	free_arr(&deployConfig[i].on_aux_run);
	free_arr(&deployConfig[i].on_aux_healthcheck);
	free_arr(&deployConfig[i].on_aux_fail);
      }
}

static config_t cfg;
/**
   @TODO instead of loading it twice, pass the config_setting back and forth?
 **/
void load_config(const char* configFile){
  printf("Loading Config at %s\n", configFile);
  config_init(&cfg);
  config_set_include_func(&cfg, include_func);  
   if (!config_read_file(&cfg, configFile)) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
        config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
	printf("Config Fail\n");
        exit(EXIT_FAILURE);
   }
  printf("Loaded Config at %s\n", configFile);   
}

void unload_config(){
  config_destroy(&cfg);
}

config_setting_t *load_deploy_config(){
  return config_lookup(&cfg,"deploy.projects");
}

int count_config(){
  const config_setting_t* projects = load_deploy_config();
  int count = config_setting_length(projects);
  return count;
}

void read_config(DeployConfig *deployConfigs) {
  // Using https://hyperrealm.github.io/libconfig  
  const config_setting_t* projects = load_deploy_config();
  
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
  } else {
    printf("No project detected!\n");
    exit(1);
  }
}

int test_config(){
  int conf_count = count_config();
  DeployConfig deployConfig[conf_count];
  read_config(&deployConfig);
  printf("Assigned MEM %i\n", sizeof(deployConfig));
  destroy_deploy_conf(&deployConfig, conf_count);
  unload_config();
  printf("Config Ok\n");
  return 0;
}

int apply() {
  printf("Queued! Ok\n");
  return 0;
}
