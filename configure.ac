AC_INIT([Unnamed Blocks], 0.1)
AM_INIT_AUTOMAKE([subdir-objects foreign])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIRS([m4])

# Detect the target system
AC_CANONICAL_HOST
case $host_os in
    linux*)
		AC_DEFINE([LINUX])
        ;;
    cygwin*|mingw*)
        ;;
	*)
        AC_MSG_ERROR(["OS $host_os is not supported"])
		;;
esac
PKG_CHECK_MODULES(GLEW, glew)
PKG_CHECK_MODULES(SFML, sfml-all)

LT_INIT
AC_PROG_CC
AC_PROG_CXX
AC_OUTPUT
