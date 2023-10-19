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

#define DD_NEW_LINE "\n"
#define DD_DEFAULT_LOG "/etc/docker-deploy/debug.log"
#define DD_DEFAULT_PID "/etc/docker-deploy/pid.id"
#define DD_STRING_STOPPING "Stopping Daemon..."
#define DD_DEFAULT_CONF "main.conf"
#define DD_NAME "Docker Doeploy"

typedef enum {
  false = 0,
  true = 1
} bool;

#define false 0
#define true 1

typedef char* string;

#include <stdio.h>
#include "stdlib.h"
#include <string.h>
#include "options.c"
#include <libconfig.h>
#include <dirent.h>
#include <fnmatch.h>
#include <limits.h>
#include <sys/stat.h>
#include "unistd.h"
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <errno.h>

#include "file.c"
#include "proc.c"
#include "conf.c"
