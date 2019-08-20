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

#include "libtest/cr_template.h"
#include "libtest/grab-logging.h"
#include <criterion/criterion.h>

#include "apphook.h"
#include "plugin.h"
#include "cfg.h"
#include "logmsg/logmsg.h"
#include "scratch-buffers.h"

void
setup(void)
{
  app_startup();
  init_template_tests();
  cfg_load_module(configuration, "examples");
}

void
teardown(void)
{
  deinit_template_tests();
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
