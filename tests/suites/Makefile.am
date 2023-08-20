check_PROGRAMS = \
	util_test \
	logger_test \
	cgentest_test \
	parser_test

util_test_SOURCES = util_test.c mocks/logger.c
logger_test_SOURCES = logger_test.c
cgentest_test_SOURCES = cgentest_test.c $(top_srcdir)/src/util.c mocks/logger.c mocks/parser.c mocks/writer.c
parser_test_SOURCES = parser_test.c mocks/util.c mocks/logger.c

AM_CPPFLAGS = -I$(top_builddir)/src -I$(top_srcdir)/src -DFIXTURE_PATH='"$(abs_top_srcdir)/tests/fixtures"' -DINSTALLED_TEMPLATE_PATH='"$(pkgdatadir)/template"' -DLOCAL_TEMPLATE_PATH='"$(abs_top_srcdir)/data/template"' -g3 -O0
