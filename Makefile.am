#
# Copyright (c) 2005, 2006, 2007 Thorsten Kukuk <kukuk@thkukuk.de>
# Copyright (c) 2011 Red Hat, Inc.
# Copyright (c) 2011 Tomas Mraz <tm@t8m.info>
#

AUTOMAKE_OPTIONS = 1.9 gnu dist-bzip2 check-news

SUBDIRS = po src doc

CLEANFILES = *~

M4_FILES = m4/libtool.m4

EXTRA_DIST = config.rpath $(M4_FILES) libpwpwned.spec libpwpwned.spec.in

ACLOCAL_AMFLAGS = -I m4

tag: Makefile
	hg tag libpwpwned-@PACKAGE_VERSION@

force-tag: Makefile
	hg tag -f libpwpwned-@PACKAGE_VERSION@
