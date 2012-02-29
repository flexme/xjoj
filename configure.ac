#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
#AC_INIT(FULL-PACKAGE-NAME, VERSION, BUG-REPORT-ADDRESS)
AC_INIT(xjoj, 1.0)
AM_INIT_AUTOMAKE(xjoj, 1.0)
AC_CONFIG_SRCDIR([lib/thread.h])
AC_CONFIG_HEADER([makeconfig.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h fcntl.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h syslog.h unistd.h sys/wait.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

# checking mysql_config
AC_PATH_PROG([mysql_config_path], [mysql_config], [no])
if test "$mysql_config_path" = "no"; then
    AC_MSG_ERROR([*** mysql_config mis required, please install mysql])
else

    #get mysql compiling info
    MYSQL_CFLAGS="$($mysql_config_path --cflags)"
    MYSQL_LDFLAGS="$($mysql_config_path --libs)"
    AC_SUBST(MYSQL_CFLAGS)
    AC_SUBST(MYSQL_LDFLAGS)
fi

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_FORK
AC_PROG_GCC_TRADITIONAL
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([bzero dup2 wait4 getcwd gettimeofday localtime_r memset mkdir rmdir select socket strdup strerror strstr])

AC_PREFIX_DEFAULT(/usr/local/xjoj)
if test "x$prefix" = xNONE; then
    XJOJHOME="/usr/local/xjoj"
else
    XJOJHOME=$prefix
fi
AC_DEFINE_UNQUOTED([XJOJHOME], "$XJOJHOME", [Path to XJOJ installation directory])

AC_CONFIG_FILES([Makefile
                 client/Makefile
                 lib/Makefile
                 server/Makefile])
AC_OUTPUT
