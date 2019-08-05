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

#include <limits.h>
#include <math.h>

static GString *
_convert(const char *format, long int num)
{
  gsize max_digits = floor(log10(LONG_MAX));
  gchar *converted_str = g_malloc(max_digits+1);

  snprintf(converted_str, max_digits, format, num);

  GString *converted_gstring = g_string_new(converted_str);
  g_free(converted_str);

  return converted_gstring;
}

static void
tf_base_dec(LogMessage *msg, gint argc, GString *argv[], GString *result)
{
  long int original = strtol(argv[0]->str, NULL, 0);
  GString *converted = _convert("%d", original);
  g_string_append(result, converted->str);
  g_string_free(converted, TRUE);
}

TEMPLATE_FUNCTION_SIMPLE(tf_base_dec);

static void
tf_base_hex(LogMessage *msg, gint argc, GString *argv[], GString *result)
{
  long int original = strtol(argv[0]->str, NULL, 0);
  GString *converted = _convert("%x", original);
  g_string_append(result, converted->str);
  g_string_free(converted, TRUE);
}

TEMPLATE_FUNCTION_SIMPLE(tf_base_hex);

static void
tf_base_oct(LogMessage *msg, gint argc, GString *argv[], GString *result)
{
  long int original = strtol(argv[0]->str, NULL, 0);
  GString *converted = _convert("%o", original);
  g_string_append(result, converted->str);
  g_string_free(converted, TRUE);
}

TEMPLATE_FUNCTION_SIMPLE(tf_base_oct);
