AC_INIT([cprog10l3],[1],[torandi@gmail.com])

AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

AC_GNU_SOURCE
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CXX
AC_PROG_RANLIB

AC_LANG_PUSH([C++])
AX_CHECK_COMPILE_FLAG([-std=c++0x], [CXXFLAGS="$CXXFLAGS -std=c++0x"], [AC_MSG_ERROR([Support for C++11 is required])])
AC_LANG_POP


AC_CHECK_HEADERS([getopt.h])


AM_PROG_AR

AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_FUNCS([access gettimeofday usleep setitimer])

AC_OUTPUT
