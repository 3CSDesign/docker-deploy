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
#include "unistd.h"
#include <signal.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <errno.h>

static int running = 0;
static char *pid_file_name = NULL;
static int pid_fd = -1;
static int counter = 0;
static FILE *log_stream;

void handle_signal(int sig){
  if (sig == SIGINT){

    printf("Stopping Daemon...\n");
    if (pid_fd != -1) {
      lockf(pid_fd, F_ULOCK, 0);
      close(pid_fd);
      if (pid_file_name != NULL) {
	unlink(pid_file_name);
      }
      running = 0;

      signal(SIGINT, SIG_DFL);
    }
  }
}

static void daemonize()
{
	pid_t pid = 0;
	int fd;

	/* Fork off the parent process */
	pid = fork();

	/* An error occurred */
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	/* Success: Let the parent terminate */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	/* On success: The child process becomes session leader */
	if (setsid() < 0) {
		exit(EXIT_FAILURE);
	}

	/* Ignore signal sent from child to parent process */
	signal(SIGCHLD, SIG_IGN);

	/* Fork off for the second time*/
	pid = fork();

	/* An error occurred */
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	/* Success: Let the parent terminate */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	/* Set new file permissions */
	umask(0);

	/* Change the working directory to the root directory */
	/* or another appropriated directory */
	chdir("/");

	/* Close all open file descriptors */
	for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
		close(fd);
	}

	/* Reopen stdin (fd = 0), stdout (fd = 1), stderr (fd = 2) */
	stdin = fopen("/dev/null", "r");
	stdout = fopen("/dev/null", "w+");
	stderr = fopen("/dev/null", "w+");

	/* Try to write PID of daemon to lockfile */
	if (pid_file_name != NULL)
	{
		char str[256];
		pid_fd = open(pid_file_name, O_RDWR|O_CREAT, 0640);
		if (pid_fd < 0) {
			/* Can't open lockfile */
			exit(EXIT_FAILURE);
		}
		if (lockf(pid_fd, F_TLOCK, 0) < 0) {
			/* Can't lock file */
			exit(EXIT_FAILURE);
		}
		/* Get current PID */
		sprintf(str, "%d\n", getpid());
		/* Write PID to lockfile */
		write(pid_fd, str, strlen(str));
	}
}


int main (int argc, char **argv){
  options_arg_type args_info;
  
  if (options(argc, argv, &args_info) != 0)
      exit(1);
  int conf_count;
   char* username;
   username = malloc(strlen(getlogin()));
   strcpy(username, getlogin());

  conf_count = count_config("main.conf");
  DeployConfig deployConfig[conf_count];
  read_config("main.conf", &deployConfig);

  int i;
   for (i = 0; i < conf_count; i++){
     DeployConfig conf = deployConfig[i];
     printf("Name %s\n", conf.name);
     printf("--> %s\n", conf.main_file);
   }

   printf("Username ->:%s\n", getlogin() );

   int start_daemonized = 0;
   if (start_daemonized == 1) {
        daemonize();
    }
   openlog("Docker Deploy", LOG_PID|LOG_CONS, LOG_DAEMON);
   syslog(LOG_INFO, "Started Docker Deploy");

   signal(SIGINT, handle_signal);
   signal(SIGHUP, handle_signal);

   char *log_file_name = "/mnt/part2/code/3cs/docker-deploy/debug.log";
   
   if (log_file_name != NULL) {
     log_stream = fopen(log_file_name, "a+");
     if (log_stream == NULL) {
       syslog(LOG_ERR, "Can not open log file: %s, error: %s",
	      log_file_name, strerror(errno));
       log_stream = stdout;
     }
   } else {
     log_stream = stdout;
   }

   running = 1;

   int ret;
   while (running == 1) {
		ret = fprintf(log_stream, "Debug: %d\n", counter++);
		if (ret < 0) {
			syslog(LOG_ERR, "Can not write to log stream: %s, error: %s",
				(log_stream == stdout) ? "stdout" : log_file_name, strerror(errno));
			break;
		}
		ret = fflush(log_stream);
		if (ret != 0) {
			syslog(LOG_ERR, "Can not fflush() log stream: %s, error: %s",
				(log_stream == stdout) ? "stdout" : log_file_name, strerror(errno));
			break;
		}
		sleep(1);
		running = 0;
    }

   	/* Close log file, when it is used. */
	if (log_stream != stdout) {
		fclose(log_stream);
	}

	/* Write system log and close it. */
	syslog(LOG_INFO, "Stopped %s", "Docker Deploy");
	closelog();

	/* Free allocated memory */
	//if (conf_file_name != NULL) free(conf_file_name);
	if (log_file_name != NULL) free(log_file_name);
	if (pid_file_name != NULL) free(pid_file_name);

	return EXIT_SUCCESS;

}
