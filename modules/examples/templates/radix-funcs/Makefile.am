noinst_LTLIBRARIES += modules/examples/templates/libradix-funcs.la

modules_examples_templates_libradix_funcs_la_SOURCES = \
  modules/examples/templates/radix-funcs/radix-funcs.c \
  modules/examples/templates/radix-funcs/radix-funcs.h


EXTRA_DIST += \
modules_examples_templates_libradix_funcs_la_CPPFLAGS  = \
  $(AM_CPPFLAGS) \
  -I$(top_srcdir)/modules/examples/templates/radix-funcs \
  -I$(top_builddir)/modules/examples/templates/radix-funcs

modules_examples_templates_libradix_funcs_la_LIBADD  = $(MODULE_DEPS_LIBS)
modules_examples_templates_libradix_funcs_la_LDFLAGS = $(MODULE_LDFLAGS)
modules_examples_templates_libradix_funcs_la_DEPENDENCIES = $(MODULE_DEPS_LIBS)

include modules/examples/templates/radix-funcs/tests/Makefile.am
