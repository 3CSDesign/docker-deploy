typedef struct {
  // Name of the project which is also the user 
    char name[100];
  // path to the main docker compose file
    char main_file[100];
  // path to the aux docker compose file
    char aux_file[100];
  // bash script to run during the main startup
    char on_main_run[1000];
  // bash to check if the main containers are online
    char on_main_healthcheck[1000];
  // bash script to exec on fail of main
    char on_main_fail[1000];

    char on_aux_run[1000];
    char on_aux_healthcheck[1000];
    char on_aux_fail[1000];

} DeployConfig;


void read_config(const char* configFile, DeployConfig* config) {

  // Using https://hyperrealm.github.io/libconfig
  
  config_t cfg;
  config_init(&cfg);
  
  // IF config fails

   if (!config_read_file(&cfg, configFile)) {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
        config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
   }
  
  const config_setting_t* root = config_root_setting(&cfg);

  
  const config_setting_t* project = config_setting_get_member(root, "project");
  if (project != NULL) {
        config_setting_lookup_string(project, "name", config->name);
        config_setting_lookup_string(project, "main_file", config->main_file);
        config_setting_lookup_string(project, "aux_file", config->aux_file);

        config_setting_t* on_main_run = config_setting_get_member(project, "on_main_run");
        if (on_main_run != NULL) {
            for (int i = 0; i < config_setting_length(on_main_run); i++) {
                const char* line = config_setting_get_string_elem(on_main_run, i);
                strncat(config->on_main_run, line, sizeof(config->on_main_run)-strlen(config->on_main_run)-1);
                strncat(config->on_main_run, "\n", sizeof(config->on_main_run)-strlen(config->on_main_run)-1);
            }
        }

        // Add similar code to read the rest of the variables
    }
   config_destroy(&cfg);
}
