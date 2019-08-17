modules_examples_templates_radix_funcs_tests_TESTS = \
modules/examples/templates/radix-funcs/tests/test_radix_funcs

check_PROGRAMS += ${modules_examples_templates_radix_funcs_tests_TESTS}

modules_examples_templates_radix_funcs_tests_test_radix_funcs_CFLAGS = \
  $(TEST_CFLAGS) -I$(top_srcdir)/modules/examples/templates/radix-funcs

modules_examples_templates_radix_funcs_tests_test_radix_funcs_LDADD  = \
  $(TEST_LDADD) \
  $(top_builddir)/modules/examples/templates/libradix-funcs.la

EXTRA_DIST += modules/examples/templates/radix-funcs/tests/CMakeLists.txt
