/*
* Copyright (c) 2019 Victor Ma
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 2 as published
* by the Free Software Foundation, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
* As an additional exemption you are allowed to compile & link against the
* OpenSSL libraries as published by the OpenSSL project. See the file
* COPYING for details.
*
*/

/* TODO: NOT WORKING */

#include "libtest/cr_template.h"
#include "libtest/grab-logging.h"
#include <criterion/criterion.h>

#include "apphook.h"
#include "plugin.h"
#include "cfg.h"
#include "logmsg/logmsg.h"
#include "scratch-buffers.h"

static void
add_dummy_template_to_configuration(void)
{
  LogTemplate *dummy = log_template_new(configuration, "dummy");
  cr_assert(log_template_compile(dummy, "dummy template expanded $HOST", NULL),
            "Unexpected error compiling dummy template");
  cfg_tree_add_template(&configuration->tree, dummy);
}

static void
_log_msg_free(gpointer data, gpointer user_data)
{
  log_msg_unref((LogMessage *) data);
}

static GPtrArray *
create_log_messages_with_values(const gchar *name, const gchar **values)
{
  LogMessage *message;
  GPtrArray *messages = g_ptr_array_new();

  const gchar **value;
  for (value = values; *value != NULL; ++value)
    {
      message = create_empty_message();
      log_msg_set_value_by_name(message, name, *value, -1);
      g_ptr_array_add(messages, message);
    }

  return messages;
}

static void
free_log_message_array(GPtrArray *messages)
{
  g_ptr_array_foreach(messages, _log_msg_free, NULL);
  g_ptr_array_free(messages, TRUE);
}

void
setup(void)
{
  app_startup();
  init_template_tests();
  add_dummy_template_to_configuration();
  cfg_load_module(configuration, "examples");
}

void
teardown(void)
{
  deinit_template_tests();
  scratch_buffers_explicit_gc();
  app_shutdown();
}

TestSuite(radix_funcs, .init = setup, .fini = teardown);

Test(radix_funcs, test_radix_funcs)
{
  assert_template_format("$(dec)", "");
  assert_template_format("$(dec 10)", "10");
  assert_template_format("$(dec 0x10)", "16");
  assert_template_format("$(dec 010)", "8");

  assert_template_format("$(hex)", "");
  assert_template_format("$(hex 10)", "a");
  assert_template_format("$(hex 0x10)", "10");
  assert_template_format("$(hex 010)", "8");

  assert_template_format("$(oct)", "");
  assert_template_format("$(oct 10)", "12");
  assert_template_format("$(oct 0x10)", "20");
  assert_template_format("$(oct 010)", "10");

  assert_template_format("$(dec 2147483647)", "2147483647");
  assert_template_format("$(hex 0x7fffffff)", "0x7fffffff");
  assert_template_format("$(oct 017777777777)", "017777777777");
  assert_template_format("$(dec 2147483648)", "");
  assert_template_format("$(hex 0x80000000)", "");
  assert_template_format("$(oct 020000000000)", "");

  assert_template_format("$(dec 10a)", "");
  assert_template_format("$(hex 0x0x)", "");
  assert_template_format("$(oct 09)", "");
}
