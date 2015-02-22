#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([echo_lake], [0.01a], [andross@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/echo_lake.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([SDL2], [main])
AC_CHECK_LIB([SDL2_gfx], [main])
AC_CHECK_LIB([SDL2_ttf], [main])
AC_CHECK_LIB([SDL2_image], [main])
AC_CHECK_LIB([jansson], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h uthash.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
