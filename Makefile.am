bin_PROGRAMS = src/cgentest
dist_pkgdata_DATA = data/template
src_cgentest_SOURCES = src/cgentest.c src/cli.c src/ctags_helper.c src/logger.c src/logger.h src/main.c src/mapper.c src/util.c
AM_CPPFLAGS = -DINSTALLED_TEMPLATE_PATH='"$(pkgdatadir)/template"' -DLOCAL_TEMPLATE_PATH='"$(abs_top_srcdir)/data/template"'

compile_commands.json: Makefile
	bear -- make --always-make
