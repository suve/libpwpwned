dnl Process this file with autoconf to produce a configure script.
AC_INIT([libpwpwned], [1.4.0])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([dist-bzip2 no-dist-gzip -Wall])
AC_PREREQ(2.61)
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_HOST

m4_include([m4/libcurl.m4])
m4_include([m4/m4_ax_check_openssl.m4])

dnl We don't build with srcdir != builddir at present; the Python
dnl bindings fail
echo \#buildapi-variable-no-builddir > /dev/null

dnl LT Version numbers, remember to change them *before* a release.
dnl   (Interfaces removed:    CURRENT++, AGE=0, REVISION=0)
dnl   (Interfaces added:      CURRENT++, AGE++, REVISION=0)
dnl   (No interfaces changed:                   REVISION++)
PWQUALITY_LT_CURRENT=1
PWQUALITY_LT_AGE=0
PWQUALITY_LT_REVISION=2

AC_SUBST(PACKAGE)
AC_SUBST(VERSION)
AC_SUBST(PWQUALITY_LT_CURRENT)
AC_SUBST(PWQUALITY_LT_AGE)
AC_SUBST(PWQUALITY_LT_REVISION)

dnl Checks for programs.
AC_USE_SYSTEM_EXTENSIONS
AM_PROG_AR
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_LIBTOOL

dnl and some hacks to use /etc
test "${prefix}" = "NONE" && prefix="/usr"
if test ${prefix} = '/usr'
then
dnl If we use /usr as prefix, use /etc for config files
        if test ${sysconfdir} = '${prefix}/etc'
        then
                sysconfdir="/etc"
        fi
dnl If we use /usr as prefix, use /usr/share/man for manual pages
        if test ${mandir} = '${prefix}/man'
        then
                mandir='${prefix}/share/man'
        fi
fi

dnl Localization support
AM_GNU_GETTEXT_VERSION([0.18.2])
AM_GNU_GETTEXT([external])
AH_BOTTOM([#ifdef ENABLE_NLS
#include <libintl.h>
#define _(msgid) dgettext(PACKAGE, msgid)
#define N_(msgid) msgid
#else   
#define _(msgid) (msgid)
#define N_(msgid) msgid
#endif /* ENABLE_NLS */])

dnl Library version scripts

dnl For some systems we know that we have ld_version scripts.
dnl Use it then as default.
have_ld_version_script=no
case "${host}" in
    *-*-linux*)
        have_ld_version_script=yes
        ;;
    *-*-gnu*)
        have_ld_version_script=yes
        ;;
esac

AC_ARG_ENABLE([ld-version-script],
              AC_HELP_STRING([--enable-ld-version-script],
                             [enable/disable use of linker version script.
                              (default is system dependent)]),
              [have_ld_version_script=$enableval],
              [ : ] )
AM_CONDITIONAL(HAVE_LD_VERSION_SCRIPT, test "$have_ld_version_script" = "yes")

dnl PAM support
PAM_LIBS=
AC_ARG_ENABLE([pam],
  AS_HELP_STRING([--enable-pam],[build PAM module @<:@default=check@:>@]),
  [],
  [enable_pam=check])
if test "$enable_pam" != "no"; then
  if test "$enable_pam" = "check"; then
      AC_CHECK_LIB([pam], [pam_start], [enable_pam=yes], [enable_pam=no])
  else
      fail=0
      AC_CHECK_LIB([pam], [pam_start], [], [fail=1])
      test $fail = 1 &&
        AC_MSG_ERROR([You must install the PAM development package in order to compile libpwpwned])
  fi
fi

if test "$enable_pam" = "yes"; then
  PAM_LIBS="${PAM_LIBS} -lpam"
fi
AC_SUBST(PAM_LIBS)
AM_CONDITIONAL(HAVE_PAM, test "$enable_pam" = "yes")

dnl options and defaults
AC_ARG_WITH(securedir,
        AS_HELP_STRING([--with-securedir=DIR],[path to location of PAMs @<:@default=$libdir/security@:>@]),
        SECUREDIR=$withval, SECUREDIR=$libdir/security)
AC_SUBST(SECUREDIR)

AC_ARG_WITH(sconfigdir,
	AS_HELP_STRING([--with-sconfigdir=DIR],[path to PAM conf files @<:@default=$sysconfdir/security@:>@]),
	SCONFIGDIR=$enableval, SCONFIGDIR=$sysconfdir/security)
AC_SUBST(SCONFIGDIR)

AC_ARG_WITH([randomdev], AS_HELP_STRING([--with-randomdev=(<path>|yes)],[use specified random device instead of /dev/urandom]), opt_randomdev=$withval)
if test "$opt_randomdev" = yes -o -z "$opt_randomdev"; then
       opt_randomdev="/dev/urandom"
fi
AC_DEFINE_UNQUOTED(CONF_PATH_RANDOMDEV, "$opt_randomdev", [Random device path.])

LIBCURL_CHECK_CONFIG([yes], [7.0.0], [], [
    AC_MSG_ERROR([No or unusable libcurl])
])


#AC_CHECK_HEADERS([openssl/sha.h],
#    AC_CHECK_LIB([ssl], ['SHA1'], LIBSSL='-lssl', LIBSSL=''))
AX_CHECK_OPENSSL(LIBSSL='-lssl', LIBSSL='')
if test x$LIBSSL = x ; then
    AC_MSG_ERROR([No or unusable openssl library])
fi


dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN

dnl Files to be created from when we run configure
AC_CONFIG_FILES([Makefile libpwpwned.spec src/Makefile src/pwpwned.pc po/Makefile.in
                 doc/Makefile doc/man/Makefile])
AC_OUTPUT
