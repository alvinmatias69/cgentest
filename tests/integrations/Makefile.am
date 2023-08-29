check_PROGRAMS = \
	jsonc_test

jsonc_test_SOURCES = $(top_srcdir)/src/cgentest.c $(top_srcdir)/src/cli.c $(top_srcdir)/src/parser.c $(top_srcdir)/src/logger.c $(top_srcdir)/src/main.c $(top_srcdir)/src/util.c $(top_srcdir)/src/writer_jsonc.c

AM_CPPFLAGS = -I$(top_srcdir) -I$(abs_top_srcdir)
AM_CPPFLAGS += -DINSTALLED_TEMPLATE_PATH='"$(pkgdatadir)/template"' -DLOCAL_TEMPLATE_PATH='"$(abs_top_srcdir)/data/template"'
AM_CPPFLAGS += -g3 -O0
