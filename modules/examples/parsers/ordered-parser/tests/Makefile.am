modules_examples_parsers_ordered_parser_tests_TESTS	= \
	modules/examples/parsers/ordered-parser/tests/test_ordered_parser

check_PROGRAMS += ${modules_examples_parsers_ordered_parser_tests_TESTS}

modules_examples_parsers_ordered_parser_tests_test_ordered_parser_CFLAGS	=	\
	$(TEST_CFLAGS) -I$(top_srcdir)/modules/examples/parsers/ordered-parser

modules_examples_parsers_ordered_parser_tests_test_ordered_parser_LDADD		=	\
	$(TEST_LDADD)

modules_examples_parsers_ordered_parser_tests_test_ordered_parser_LDFLAGS	=	\
	$(PREOPEN_SYSLOGFORMAT) -dlpreopen $(top_builddir)/modules/examples/parsers/libordered-parser.la

modules_examples_parsers_ordered_parser_tests_test_ordered_parser_DEPENDENCIES = \
	$(top_builddir)/modules/examples/parsers/libordered-parser.la

EXTRA_DIST += modules/examples/parsers/ordered-parser/tests/CMakeLists.txt
