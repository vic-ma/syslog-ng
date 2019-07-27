/*
 * Copyright (c) 2012 Balabit
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

static void
tf_env(LogMessage *msg, gint argc, GString *argv[], GString *result)
{
  gint i;

  for (i = 0; i < argc; i++)
    {
      char *val = getenv(argv[i]->str);

      if (!val)
        continue;

      g_string_append(result, val);
      if (i < argc - 1)
        g_string_append_c(result, ' ');
    }
}

TEMPLATE_FUNCTION_SIMPLE(tf_env);

static void
tf_ietf_to_bsd(LogMessage *msg, gint argc, GString *argv[], GString *result)
{
  const gchar *pri = log_msg_get_value_by_name(msg, "PRI", NULL);
  g_string_append_c(result, '<');
  g_string_append(result, pri);
  g_string_append_c(result, '>');

  const gchar *date = log_msg_get_value_by_name(msg, "DATE", NULL);
  g_string_append(result, date);
  g_string_append_c(result, ' ');

  const gchar *host = log_msg_get_value(msg, LM_V_HOST, NULL);
  g_string_append(result, host);
  g_string_append_c(result, ' ');

  const gchar *msg_tag = log_msg_get_value(msg, LM_V_LEGACY_MSGHDR, NULL);
  const gchar *msg_content = log_msg_get_value(msg, LM_V_MESSAGE, NULL);
  g_string_append(result, msg_tag);
  //g_string_append(result, msg_content);
}

TEMPLATE_FUNCTION_SIMPLE(tf_ietf_to_bsd);
