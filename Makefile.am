bin_PROGRAMS = src/cgentest
dist_pkgdata_DATA = data/template
# Implement this for conditional json lib
# https://stackoverflow.com/questions/1141191/how-do-i-setup-automake-and-autoconf-to-conditionally-build-programs-tests-or-o
# src_cgentest_SOURCES = src/cgentest.c src/cli.c src/parser.c src/logger.c src/main.c src/util.c src/writer_cjson.c
src_cgentest_SOURCES = src/cgentest.c src/cli.c src/parser.c src/logger.c src/main.c src/util.c
src_cgentest_SOURCES += src/writer_jsonc.c

AM_CPPFLAGS = -DINSTALLED_TEMPLATE_PATH='"$(pkgdatadir)/template"' -DLOCAL_TEMPLATE_PATH='"$(abs_top_srcdir)/data/template"'

compile_commands.json: Makefile
	bear -- make --always-make
