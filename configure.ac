AC_INIT([Unnamed Blocks], 0.1)
AM_INIT_AUTOMAKE([subdir-objects foreign])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIRS([m4])

AX_BOOST_BASE([1.71],, [AC_MSG_ERROR([killme])])
PKG_CHECK_MODULES(GLEW, glew)
PKG_CHECK_MODULES(SFML, sfml-all)
AX_BOOST_FILESYSTEM
# Detect the target system
AC_CANONICAL_HOST
case $host_os in
    linux*)
		AC_DEFINE([LINUX])
		AC_DEFINE([POSIX])
		PKG_CHECK_MODULES(X11, x11)
        ;;
    cygwin*|mingw*)
		AC_DEFINE([POSIX])
        ;;
	*)
        AC_MSG_ERROR(["OS $host_os is not supported"])
		;;
esac
LT_INIT
AC_PROG_CC
AC_PROG_CXX
AC_OUTPUT
