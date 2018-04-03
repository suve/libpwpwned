#
# Copyright (c) 2005, 2006, 2007, 2009 Thorsten Kukuk <kukuk@thkukuk.de>
# Copyright (c) 2011 Red Hat, Inc.
# Copyright (c) 2011 Tomas Mraz <tm@t8m.info>
#

CLEANFILES = *~

securelibdir = @SECUREDIR@

secureconfdir = @SCONFIGDIR@

EXTRA_DIST = libpwpwned.map pwpwned.conf pwpwned.pc

include_HEADERS = pwpwned.h

if HAVE_LD_VERSION_SCRIPT
  libpwpwned_version_script = -Wl,--version-script=$(srcdir)/libpwpwned.map
else
  libpwpwned_version_script =
endif

libpwpwned_la_LDFLAGS = -no-undefined $(libpwpwned_version_script) \
	-version-info @PWQUALITY_LT_CURRENT@:@PWQUALITY_LT_REVISION@:@PWQUALITY_LT_AGE@

libpwpwned_la_LIBADD = $(LIBCRACK) $(LIBINTL) $(SSH2_LIBS) $(CURL_LIBS)

libpwpwned_la_SOURCES = check.c error.c

if HAVE_PAM
  pam_pwpwned_la_LDFLAGS = -no-undefined -avoid-version -module
  pam_pwpwned_la_LIBADD = libpwpwned.la $(PAM_LIBS) $(LIBINTL)
  pam_pwpwned_la_SOURCES = pam_pwpwned.c
else
  pam_pwpwned_la_LDFLAGS =
  pam_pwpwned_la_LIBADD =
  pam_pwpwned_la_SOURCES =
endif

lib_LTLIBRARIES = libpwpwned.la

if HAVE_PAM
  securelib_LTLIBRARIES = pam_pwpwned.la
else
  securelib_LTLIBRARIES =
endif

secureconf_DATA = pwpwned.conf

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pwpwned.pc
