#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([cgentest], [1.0.0], [cgentest@matiasalvin.dev])
AC_CONFIG_SRCDIR([src/cgentest.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])

AC_CONFIG_TESTDIR([tests])
AC_CONFIG_FILES([tests/Makefile
                 tests/suites/Makefile])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
have_readtags=no
AC_CHECK_HEADER([readtags.h], [have_readtags=yes])

AS_IF([test "x${have_readtags}" = "xyes"],
         [AC_SEARCH_LIBS([tagsFind], [readtags], [], [have_readtags=no])])

AS_IF([test "x${have_readtags}" = "xno"],
         [AC_MSG_ERROR([
   The readtags library and header files are
   required to build cgentest. Stopping...
   Check 'config.log' for more information.
   ------------------------------------------])])

# check user input
no_preference=yes
use_jsonc=no
AC_ARG_WITH([jsonc],
   [AS_HELP_STRING([--with-jsonc],
      [use json-c for json library @<:@default=no@:>@])],
      [use_jsonc=yes]
      [no_preference=no])
if test "x${use_jsonc}" = xyes; then
   AC_MSG_NOTICE([
   Using json-c as json library])
fi

use_cjson=no
AC_ARG_WITH([cjson],
   [AS_HELP_STRING([--with-cjson],
      [use cjson for json library @<:@default=no@:>@])],
      [use_cjson=yes]
      [no_preference=no])
if test "x${use_cjson}" = xyes; then
   AC_MSG_NOTICE([
   Using cjson as json library])
fi

use_jansson=no
AC_ARG_WITH([jansson],
   [AS_HELP_STRING([--with-jansson],
      [use jansson for json library @<:@default=no@:>@])],
      [use_jansson=yes]
      [no_preference=no])
if test "x${use_jansson}" = xyes; then
   AC_MSG_NOTICE([
   Using jansson as json library])
fi

if test "x${no_preference}" = xyes; then
   AC_MSG_NOTICE([
   User doesn't give json library preference.
   Will check for installed library.])
fi

lib_set=no

# check jsonc availability and link if available
have_mustach_jsonc=no
AS_IF([test "x${lib_set}" = "xyes"],
         [AC_MSG_NOTICE([libraries already set, skipping jsonc check])],
      [test "x${use_jsonc}" = "xyes" -o "x${no_preference}" = "xyes"],
         [AC_CHECK_HEADER([mustach/mustach-json-c.h], [have_mustach_jsonc=yes])])

AS_IF([test "x${have_mustach_jsonc}" = "xyes"],
         [AC_SEARCH_LIBS([mustach_json_c_file], [mustach], [], [have_mustach_jsonc=no])])

have_jsonc=no
AS_IF([test "x${have_mustach_jsonc}" = "xyes"],
         [AC_CHECK_HEADER([json-c/json_object.h], [have_jsonc=yes])])

AS_IF([test "x${have_jsonc=}" = "xyes"],
         [AC_SEARCH_LIBS([json_object_new_object], [json-c], [], [have_jsonc=no])])

AS_IF([test "x${have_jsonc}" = "xyes"],
         [lib_set=yes],
      [test "x${use_jsonc}" = "xyes"],
         [AC_MSG_WARN([json-c library not found])])

# check cjson availability and link if available
have_mustach_cjson=no
AS_IF([test "x${lib_set}" = "xyes"],
         [AC_MSG_NOTICE([libraries already set, skipping cjson check])],
      [test "x${use_cjson}" = "xyes" -o "x${no_preference}" = "xyes"],
         [AC_CHECK_HEADER([mustach/mustach-cjson.h], [have_mustach_cjson=yes])])

AS_IF([test "x${have_mustach_cjson}" = "xyes"],
         [AC_SEARCH_LIBS([mustach_cJSON_file], [mustach], [], [have_mustach_cjson=no])])

have_cjson=no
AS_IF([test "x${have_mustach_cjson}" = "xyes"],
         [AC_CHECK_HEADER([cjson/cJSON.h], [have_cjson=yes])])

AS_IF([test "x${have_cjson=}" = "xyes"],
         [AC_SEARCH_LIBS([cJSON_CreateObject], [cjson], [], [have_cjson=no])])

AS_IF([test "x${have_cjson}" = "xyes"],
         [lib_set=yes],
      [test "x${use_cjson}" = "xyes"],
         [AC_MSG_WARN([cjson library not found])])

# check jansson availability and link if available
have_mustach_jansson=no
AS_IF([test "x${lib_set}" = "xyes"],
         [AC_MSG_NOTICE([libraries already set, skipping jansson check])],
      [test "x${use_jansson}" = "xyes" -o "x${no_preference}" = "xyes"],
         [AC_CHECK_HEADER([mustach/mustach-jansson.h], [have_mustach_jansson=yes])])

AS_IF([test "x${have_mustach_jansson}" = "xyes"],
         [AC_SEARCH_LIBS([mustach_jansson_file], [mustach], [], [have_mustach_jansson=no])])

have_jansson=no
AS_IF([test "x${have_mustach_jansson}" = "xyes"],
         [AC_CHECK_HEADER([jansson.h], [have_jansson=yes])])

AS_IF([test "x${have_jansson=}" = "xyes"],
         [AC_SEARCH_LIBS([json_object], [jansson], [], [have_jansson=no])])

AS_IF([test "x${have_jansson}" = "xyes"],
         [lib_set=yes],
      [test "x${use_jansson}" = "xyes"],
         [AC_MSG_WARN([jansson library not found])])

# check if any lib are set
AS_IF([test "x${lib_set}" = "xno"],
         [AC_MSG_ERROR([
   To build cgentest, the presence of at least
   one JSON library (json-c, cjson, jansson) and
   its associated header files is required. Stopping...
   Check 'config.log' for more information.
   ------------------------------------------])])

AM_CONDITIONAL([USE_JSONC], [test "x${have_jsonc}" = "xyes"])
AM_CONDITIONAL([USE_CJSON], [test "x${have_cjson}" = "xyes"])
AM_CONDITIONAL([USE_JANSSON], [test "x${have_jansson}" = "xyes"])

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

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
