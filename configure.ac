dnl Process this file with autoconf to produce the configure script.
dnl This line is a comment.

dnl Project Info
AC_INIT(server, 2.0, staff@middle-earth.us)

dnl Config Dir
AC_CONFIG_AUX_DIR(config)

dnl Use automake
AM_INIT_AUTOMAKE(server, 2.0)

dnl Config Header
AM_CONFIG_HEADER(config.h)

dnl Compiler flags
CPPFLAGS="-ggdb -DLINUX -O2 -Wall"
AC_SUBST(CPPFLAGS)

dnl Check for typical progs
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_LANG([C++])

dnl Check for libs
AC_CHECK_LIB(mysqlclient, mysql_init)
AC_CHECK_LIB(m,roundf)
AC_CHECK_LIB(crypt,crypt)
AC_CHECK_LIB(nsl,gethostbyname)

AC_OUTPUT(Makefile rpi_engine/Makefile)