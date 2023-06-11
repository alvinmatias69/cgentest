#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([cgentest], [1.0.0], [cgentest@matiasalvin.dev])
AC_CONFIG_SRCDIR([src/cgentest.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
have_readtags=no
AC_SEARCH_LIBS([tagsFind], [readtags], [have_readtags=yes])

if test "x${have_readtags}" = xyes; then
   AC_CHECK_HEADERS([readtags.h], [], [have_readtagss=no])
fi

if test "x${have_readtags}" = xno; then
   AC_MSG_ERROR([
   The readtags library and header files are
   required to build cgentest. Stopping...
   Check 'config.log' for more information.
   ------------------------------------------])
fi

have_cjson=no
AC_SEARCH_LIBS([cJSON_CreateObject], [cjson], [have_cjson=yes])

if test "x${have_cjson}" = xyes; then
   AC_CHECK_HEADERS([cjson/cJSON.h], [], [have_cjson=no])
fi

if test "x${have_cjson}" = xno; then
   AC_MSG_ERROR([
   The cJSON library and header files are
   required to build cgentest. Stopping...
   Check 'config.log' for more information.
   ------------------------------------------])
fi

have_mustach=no
AC_SEARCH_LIBS([mustach_cJSON_file], [mustach], [have_mustach=yes])

if test "x${have_mustach}" = xyes; then
   AC_CHECK_HEADERS([mustach/mustach-cjson.h], [], [have_mustach=no])
fi

if test "x${have_mustach}" = xno; then
   AC_MSG_ERROR([
   The mustach library and header files are
   required to build cgentest. Stopping...
   Check 'config.log' for more information.
   ------------------------------------------])
fi

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UID_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_FUNC_OBSTACK
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_STRUCT_TIMEZONE

# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_FORK
AC_FUNC_FSEEKO
AC_FUNC_GETGROUPS
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_FUNC_STRCOLL
AC_FUNC_STRERROR_R
AC_FUNC_STRNLEN
AC_FUNC_STRTOD
AC_CHECK_FUNCS([alarm atexit bzero clock_gettime dup2 euidaccess fchdir fdatasync ftruncate getcwd getdelim gethostname getpagesize getpass getusershell lchown localtime_r mblen memchr memmove mempcpy memset pathconf putenv realpath rmdir rpmatch select setenv sethostname stpcpy strcasecmp strchr strcspn strdup strerror strncasecmp strndup strpbrk strrchr strspn strstr strtol strtoul strtoull strverscmp tzset])

AC_CONFIG_MACRO_DIRS([m4])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
