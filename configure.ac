#---------------------------------------------
# Variables
#---------------------------------------------
LDFLAGS="$LDFLAGS -L/usr/local/lib"
LIBS="$LIBS -L/usr/local/lib"

#---------------------------------------------
# INIT
#---------------------------------------------

AC_PREREQ([2.71])
AC_INIT([DEDEPLOY], [0.1], [jvcooray@gmail.com], [3cs.lk], [https://3cs.lk])
AC_LANG([C])
AM_INIT_AUTOMAKE
#-------------------

AC_CONFIG_HEADERS([src/config.h])

AC_CHECK_FUNCS([strchr])
AC_FUNC_MALLOC

# Checks for programs.
AC_PROG_CC
AC_CONFIG_SRCDIR([src/main.c])

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
AC_CHECK_HEADERS([fcntl.h syslog.h])
AC_CHECK_HEADER([stdio.h],
                [AC_DEFINE([HAVE_STDIO_H], [1],
                   [Define to 1 if you have <stdio.h>.])],
                [AC_MSG_ERROR([stdio.h not found])])

PKG_CHECK_MODULES([LIBCONFIG], [libconfig >= 1.7],,
  AC_MSG_ERROR([libconfig 1.7 or newer not found.])
)


#---------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
#---------------------------------------------

AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T


 AC_CHECK_FUNCS([getcwd])
 AC_CHECK_FUNCS([strdup])
 AC_CHECK_FUNCS([strerror])
 AC_CHECK_FUNCS([strrchr])
 AC_CHECK_HEADERS([fcntl.h])
 AC_CHECK_HEADERS([syslog.h])
 AC_FUNC_FORK
 AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
 AC_FUNC_REALLOC
 AC_TYPE_PID_T

#---------------------------------------------
# Checks for library functions.
#---------------------------------------------

AC_CONFIG_FILES([makefile])
AC_OUTPUT
