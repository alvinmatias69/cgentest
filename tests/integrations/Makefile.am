check_PROGRAMS = \
	jsonc_test \
	cjson_test \
	jansson_test

INTEGRATIONCORE = $(top_srcdir)/src/cgentest.c $(top_srcdir)/src/cli.c $(top_srcdir)/src/parser.c $(top_srcdir)/src/logger.c $(top_srcdir)/src/main.c $(top_srcdir)/src/util.c

jsonc_test_SOURCES = $(INTEGRATIONCORE) $(top_srcdir)/src/writer_jsonc.c
jsonc_test_LDADD = -lmustach-json-c -ljson-c

cjson_test_SOURCES = $(INTEGRATIONCORE) $(top_srcdir)/src/writer_cjson.c
cjson_test_LDADD = -lmustach-cjson -lcjson

jansson_test_SOURCES = $(INTEGRATIONCORE) $(top_srcdir)/src/writer_jansson.c
jansson_test_LDADD = -lmustach-jansson -ljansson

AM_CPPFLAGS = -I$(top_srcdir) -I$(abs_top_srcdir)
AM_CPPFLAGS += -DINSTALLED_TEMPLATE_PATH='"$(pkgdatadir)/template"' -DLOCAL_TEMPLATE_PATH='"$(abs_top_srcdir)/data/template"'
AM_CPPFLAGS += -g3 -O0
