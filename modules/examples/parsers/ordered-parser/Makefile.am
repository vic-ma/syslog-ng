noinst_LTLIBRARIES += modules/examples/parsers/libordered-parser.la

modules_examples_parsers_libordered_parser_la_SOURCES = \
  modules/examples/parsers/ordered-parser/ordered-parser-grammar.y \
  modules/examples/parsers/ordered-parser/ordered-parser.c \
  modules/examples/parsers/ordered-parser/ordered-parser.h \
  modules/examples/parsers/ordered-parser/ordered-parser-parser.c \
  modules/examples/parsers/ordered-parser/ordered-parser-parser.h

BUILT_SOURCES += \
  modules/examples/parsers/ordered-parser/ordered-parser-grammar.y \
  modules/examples/parsers/ordered-parser/ordered-parser-grammar.c \
  modules/examples/parsers/ordered-parser/ordered-parser-grammar.h

EXTRA_DIST += \
  modules/examples/parsers/ordered-parser/ordered-parser-grammar.ym \
  modules/examples/parsers/ordered-parser/CMakeLists.txt

modules_examples_parsers_libordered_parser_la_CPPFLAGS  = \
  $(AM_CPPFLAGS) \
  -I$(top_srcdir)/modules/examples/parsers/ordered-parser \
  -I$(top_builddir)/modules/examples/parsers/ordered-parser

modules_examples_parsers_libordered_parser_la_LIBADD  = $(MODULE_DEPS_LIBS)
modules_examples_parsers_libordered_parser_la_LDFLAGS = $(MODULE_LDFLAGS)
modules_examples_parsers_libordered_parser_la_DEPENDENCIES = $(MODULE_DEPS_LIBS)

include modules/examples/parsers/ordered-parser/tests/Makefile.am
