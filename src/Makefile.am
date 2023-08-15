bin_PROGRAMS = cgentest
dist_pkgdata_DATA = $(abs_top_srcdir)/data/template

cgentest_SOURCES = cgentest.c cli.c parser.c logger.c main.c util.c

if USE_JSONC
cgentest_SOURCES += writer_jsonc.c
endif

if USE_CJSON
cgentest_SOURCES += writer_cjson.c
endif

if USE_JANSSON
cgentest_SOURCES += writer_jansson.c
endif

AM_CPPFLAGS = -DINSTALLED_TEMPLATE_PATH='"$(pkgdatadir)/template"' -DLOCAL_TEMPLATE_PATH='"$(abs_top_srcdir)/data/template"'
