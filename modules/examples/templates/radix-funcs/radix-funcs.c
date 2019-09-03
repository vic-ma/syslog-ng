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

#include <math.h>

#include "radix-funcs.h"

static gboolean
_check_argc(gint argc, const gchar *tf_name)
{
  if (argc == 0)
    {
      return FALSE;
    }
  else if (argc > 1)
    {
      msg_error("parsing failed: too many arguments", evt_tag_str("tf_name", tf_name));
      return FALSE;
    }
  return TRUE;
}

static gboolean
_check_strtol_result(gchar *endptr, const gchar *tf_name)
{
  if (*endptr != '\0' || errno == EINVAL || errno == ERANGE)
    {
      msg_error("conversion failed: invalid number", evt_tag_str("tf_name", tf_name));
      return FALSE;
    }
  return TRUE;
}

#define IMPLEMENT_RADIX_TF(radix_name, print_format)                                        \
    static void                                                                             \
    tf_radix_ ## radix_name(LogMessage *msg, gint argc, GString *argv[], GString *result)   \
    {                                                                                       \
      const gchar *tf_name = "($" #radix_name ")";                                          \
      if (!_check_argc(argc, tf_name))                                                      \
        return;                                                                             \
      gchar *endptr;                                                                        \
      errno = 0;                                                                            \
      glong original = strtol(argv[0]->str, &endptr, 0);                                    \
      if (!_check_strtol_result(endptr, tf_name))                                           \
        return;                                                                             \
      g_string_append_printf(result, #print_format, original);                              \
    }                                                                                       \
                                                                                            \
    TEMPLATE_FUNCTION_SIMPLE(tf_radix_ ## radix_name);

IMPLEMENT_RADIX_TF(dec, %ld);
IMPLEMENT_RADIX_TF(hex, %lx);
IMPLEMENT_RADIX_TF(oct, %lo);
