bin_PROGRAMS = src/cgentest
dist_pkgdata_DATA = data/template

src_cgentest_SOURCES = src/cgentest.c src/cli.c src/parser.c src/logger.c src/main.c src/util.c

if USE_JSONC
src_cgentest_SOURCES += src/writer_jsonc.c
endif

if USE_CJSON
src_cgentest_SOURCES += src/writer_cjson.c
endif

if USE_JANSSON
src_cgentest_SOURCES += src/writer_jansson.c
endif

AM_CPPFLAGS = -DINSTALLED_TEMPLATE_PATH='"$(pkgdatadir)/template"' -DLOCAL_TEMPLATE_PATH='"$(abs_top_srcdir)/data/template"'

compile_commands.json: Makefile
	bear -- make --always-make
