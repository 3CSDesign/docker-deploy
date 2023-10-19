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
  
 else if (sig == SIGHUP) {
   fprintf(log_stream, "Debug: reloading daemon config file ...\n");
   //reload_config();
 }
  else if (sig == SIGCHLD) {
   fprintf(log_stream, "Debug: received SIGCHLD signal\n");
 }
  
}

static void daemonize()
{
	pid_t pid = 0;
	int fd;
	pid = fork();
	if (pid < 0) 
	  exit(EXIT_FAILURE);
        
	if (pid > 0)
	  exit(EXIT_SUCCESS);

	if (setsid() < 0) 
	  exit(EXIT_FAILURE);
	
	signal(SIGCHLD, SIG_IGN);
	pid = fork();

	if (pid < 0)
	  exit(EXIT_FAILURE);

	if (pid > 0)
	  exit(EXIT_SUCCESS);

	umask(0);
	chdir("/");

	for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
		close(fd);
	}

	stdin = fopen("/dev/null", "r");
	stdout = fopen("/dev/null", "w+");
	stderr = fopen("/dev/null", "w+");

	if (pid_file_name != NULL)
	{
		char str[256];
		pid_fd = open(pid_file_name, O_RDWR|O_CREAT, 0640);
		if (pid_fd < 0)
		  exit(EXIT_FAILURE);
		if (lockf(pid_fd, F_TLOCK, 0) < 0)
		  exit(EXIT_FAILURE);
        	sprintf(str, "%d\n", getpid());
        	write(pid_fd, str, strlen(str));
	}
}


int main_proccess() {

   openlog(DD_NAME, LOG_PID|LOG_CONS, LOG_DAEMON);
   syslog(LOG_INFO, "Started Docker Deploy");

   signal(SIGINT, handle_signal);
   signal(SIGHUP, handle_signal);

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
   while (running != 0) {
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
    }


   	/* Close log file, when it is used. */
	if (log_stream != stdout) {
		fclose(log_stream);
	}
	/* Write system log and close it. */
	syslog(LOG_INFO, "Stopped %s", DD_NAME);
	closelog();

	/* Free allocated memory */
	if (log_file_name != NULL) free(log_file_name);
	if (pid_file_name != NULL) free(pid_file_name);

	return EXIT_SUCCESS;
}
