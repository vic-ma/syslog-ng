noinst_LTLIBRARIES += modules/examples/filters/libfilter-length.la

modules_examples_filters_libfilter_length_la_SOURCES = \
  modules/examples/filters/filter-length/filter-length-grammar.y \
  modules/examples/filters/filter-length/filter-length.c \
  modules/examples/filters/filter-length/filter-length.h \
  modules/examples/filters/filter-length/filter-length-parser.c \
  modules/examples/filters/filter-length/filter-length-parser.h

BUILT_SOURCES += \
  modules/examples/filters/filter-length/filter-length-grammar.y \
  modules/examples/filters/filter-length/filter-length-grammar.c \
  modules/examples/filters/filter-length/filter-length-grammar.h

EXTRA_DIST += \
  modules/examples/filters/filter-length/filter-length-grammar.ym \
  modules/examples/filters/filter-length/CMakeLists.txt

modules_examples_filters_libfilter_length_la_CPPFLAGS  = \
  $(AM_CPPFLAGS) \
  -I$(top_srcdir)/modules/examples/filters/filter-length \
  -I$(top_builddir)/modules/examples/filters/filter-length

modules_examples_filters_libfilter_length_la_LIBADD  = $(MODULE_DEPS_LIBS)
modules_examples_filters_libfilter_length_la_LDFLAGS = $(MODULE_LDFLAGS)
modules_examples_filters_libfilter_length_la_DEPENDENCIES = $(MODULE_DEPS_LIBS)

include modules/examples/filters/filter-length/tests/Makefile.am
