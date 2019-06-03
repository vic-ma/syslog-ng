noinst_LTLIBRARIES += modules/examples/sources/libstatic-file.la

modules_examples_sources_libmsg_generator_la_SOURCES = \
  modules/examples/sources/static-file/static-file-grammar.y \
  modules/examples/sources/static-file/static-file-parser.c \
  modules/examples/sources/static-file/static-file-parser.h \
  modules/examples/sources/static-file/static-file.c \
  modules/examples/sources/static-file/static-file.h \

BUILT_SOURCES += \
  modules/examples/sources/static-file/static-file-grammar.y \
  modules/examples/sources/static-file/static-file-grammar.c \
  modules/examples/sources/static-file/static-file-grammar.h

EXTRA_DIST += \
  modules/examples/sources/static-file/static-file-grammar.ym \
dnl  modules/examples/sources/static-file/CMakeLists.txt

modules_examples_sources_libmsg_generator_la_CPPFLAGS  = \
  $(AM_CPPFLAGS) \
  -I$(top_srcdir)/modules/examples/sources/static-file \
  -I$(top_builddir)/modules/examples/sources/static-file

modules_examples_sources_libmsg_generator_la_LIBADD  = $(MODULE_DEPS_LIBS)
modules_examples_sources_libmsg_generator_la_LDFLAGS = $(MODULE_LDFLAGS)
modules_examples_sources_libmsg_generator_la_DEPENDENCIES = $(MODULE_DEPS_LIBS)
