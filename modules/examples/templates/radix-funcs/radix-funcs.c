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
      GString *error_msg = g_string_new(tf_name);
      g_string_append(error_msg, " parsing failed: too many arguments");
      msg_error(error_msg->str);
      g_string_free(error_msg, TRUE);
      return FALSE;
    }
  return TRUE;
}

static gboolean
_check_strtol_result(gchar *endptr, const gchar *tf_name)
{
  if (*tf_name != '\0' || errno == EINVAL || errno == ERANGE)
    {
      GString *error_msg = g_string_new(tf_name);
      g_string_append(error_msg, " conversion failed: invalid number");
      msg_error(error_msg->str);
      g_string_free(error_msg, TRUE);
      return FALSE;
    }
  return TRUE;
}

static gchar *
_convert(const char *format, long int num)
{
  return g_strdup_printf(format, num);
}

#define IMPLEMENT_RADIX_TF(radix_name, print_format)                                        \
    static void                                                                             \
    tf_radix_ ## radix_name(LogMessage *msg, gint argc, GString *argv[], GString *result)   \
    {                                                                                       \
      const gchar *tf_name = "($" #radix_name ")";                                          \
      if (!_check_argc(argc, tf_name))                                                      \
        return;                                                                             \
      errno = 0;                                                                            \
      gchar *endptr = g_malloc(argv[0]->len);                                               \
      long int original = strtol(argv[0]->str, &endptr, 0);                                 \
      if (!_check_strtol_result(endptr, tf_name))                                           \
        return;                                                                             \
      gchar *converted = _convert(#print_format, original);                                 \
      g_string_append(result, converted);                                                   \
      g_free(converted);                                                                    \
    }                                                                                       \
                                                                                            \
    TEMPLATE_FUNCTION_SIMPLE(tf_radix_ ## radix_name);

IMPLEMENT_RADIX_TF(dec, %d);
IMPLEMENT_RADIX_TF(hex, %x);
IMPLEMENT_RADIX_TF(oct, %o);
