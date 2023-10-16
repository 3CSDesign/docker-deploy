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


AC_CHECK_FUNCS([strchr])
AC_FUNC_MALLOC

# Checks for programs.
AC_PROG_CC
AC_CONFIG_SRCDIR([src/main.c])

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

AC_CHECK_HEADER([stdio.h],
                [AC_DEFINE([HAVE_STDIO_H], [1],
                   [Define to 1 if you have <stdio.h>.])],
                [AC_MSG_ERROR([stdio.h not found])])

#---------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
#---------------------------------------------

AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
#---------------------------------------------
# Checks for library functions.
#---------------------------------------------

AC_CONFIG_FILES([makefile])
AC_OUTPUT
