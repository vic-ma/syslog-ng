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

#define MAX_DIGITS 100

static gboolean
_check_argc(gint argc, const gchar *tf_name)
{
  GString *error_msg = g_string_new(tf_name);

  if (argc == 0)
    {
      return FALSE;
    }
  else if (argc > 1)
    {
      g_string_append(error_msg, " parsing failed: too many arguments");
      msg_error(error_msg->str);
      g_string_free(error_msg, TRUE);
      return FALSE;
    }
  return TRUE;
}

static gchar *
_convert(const char *format, long int num)
{
  gchar *converted = g_malloc(MAX_DIGITS+1);
  snprintf(converted, MAX_DIGITS, format, num);
  return converted;
}

#define IMPLEMENT_RADIX_TF(radix_name, print_format)                                        \
    static void                                                                             \
    tf_radix_ ## radix_name(LogMessage *msg, gint argc, GString *argv[], GString *result)   \
    {                                                                                       \
      const gchar *tf_name = "($" #radix_name ")";                                          \
      if (!_check_argc(argc, tf_name))                                                      \
        return;                                                                             \
      long int original = strtol(argv[0]->str, NULL, 0);                                    \
      gchar *converted = _convert(#print_format, original);                                 \
      g_string_append(result, converted);                                                   \
      g_free(converted);                                                                    \
    }                                                                                       \
                                                                                            \
    TEMPLATE_FUNCTION_SIMPLE(tf_radix_ ## radix_name);

IMPLEMENT_RADIX_TF(dec, %d);
IMPLEMENT_RADIX_TF(hex, %x);
IMPLEMENT_RADIX_TF(oct, %o);
