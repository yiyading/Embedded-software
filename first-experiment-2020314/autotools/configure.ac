#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(program, 1.0)
#AM_INIT_AUTOMAKE需手动添加，是automake必须用到的宏，创建发布压缩包时会用到
AM_INIT_AUTOMAKE(program, 1.0)
#AC_CONFIG_SCRDIR检查指定源码目录中源码文件是否存在，可以指定所有相关源文件
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC			#AC_PROG_CC指定编译器
# Checks for libraries.
# Checks for header files.
AC_CHECK_HEADERS([string.h])
# Checks for typedefs, structures, and compiler characteristics.
# Checks for library functions.
AC_CONFIG_FILES([Makefile])	#AC_CONFIG_FILES需要手动添加，用于生成最终的Makefile文件
AC_OUTPUT
