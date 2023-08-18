check_PROGRAMS = \
	util_test \
	logger_test

util_test_SOURCES = util_test.c $(top_srcdir)/src/logger.c
logger_test_SOURCES = logger_test.c

AM_CPPFLAGS = -I$(top_builddir)/src -I$(top_srcdir)/src -DFIXTURE_PATH='"$(abs_top_srcdir)/tests/fixtures"'
