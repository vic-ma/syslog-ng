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

#include "ordered-parser.h"
#include "scanner/kv-scanner/kv-scanner.h"
#include "scratch-buffers.h"

enum
{
  OPF_LETTERS = 0x0001,
  OPF_NUMBERS = 0x0002,
};

CfgFlagHandler ordered_parser_flag_handlers[] =
{
  { "letters",        CFH_SET, offsetof(OrderedParser, flags), OPF_LETTERS},
  { "numbers",        CFH_SET, offsetof(OrderedParser, flags), OPF_NUMBERS},
  { NULL },
};

gboolean
ordered_parser_process_flag(LogParser *s, const gchar *flag)
{
  OrderedParser *self = (OrderedParser *) s;

  cfg_process_flag(ordered_parser_flag_handlers, self, flag);
}

gboolean
ordered_parser_suffix_valid(gchar suffix)
{
  return (suffix != ' '  && suffix != '\'' && suffix != '\"' );
}

void
ordered_parser_set_suffix(LogParser *s, gchar suffix)
{
  OrderedParser *self = (OrderedParser *) s;
  self->suffix = suffix;
}

void
ordered_parser_set_prefix(LogParser *s, gchar *prefix)
{
  OrderedParser *self = (OrderedParser *) s;

  g_free(self->prefix);
  if (prefix)
    {
      self->prefix = g_strdup(prefix);
      self->prefix_len = strlen(prefix);
    }
  else
    {
      self->prefix = NULL;
      self->prefix_len = 0;
    }
}

static char *
_format_input(const gchar *input, gchar suffix)
{
  GString *formatted = g_string_sized_new(strlen(input));
  gboolean finding_spaces = FALSE;
  gchar current;

  while ((current = *input++) != '\0')
    {
      if (!finding_spaces)
        {
          if (current == suffix)
            finding_spaces = TRUE;
          g_string_append_c(formatted, current);
        }
      else if (finding_spaces && current != ' ')
        {
          g_string_append_c(formatted, current);
          finding_spaces = FALSE;
        }
    }

  return g_string_free(formatted, FALSE);
}

static const gchar *
_get_formatted_key_with_prefix(OrderedParser *self, const gchar *key, GString *formatted_key)
{
  if (formatted_key->len > 0)
    g_string_truncate(formatted_key, self->prefix_len);
  else
    g_string_assign(formatted_key, self->prefix);
  g_string_append(formatted_key, key);
  return formatted_key->str;
}

static const gchar *
_get_formatted_key(OrderedParser *self, const gchar *key, GString *formatted_key)
{
  if (!self->prefix)
    return key;
  return _get_formatted_key_with_prefix(self, key, formatted_key);
}

static gboolean
_process(LogParser *s, LogMessage **pmsg, const LogPathOptions *path_options,
         const gchar *input, gsize input_len)
{
  OrderedParser *self = (OrderedParser *) s;

  KVScanner kv_scanner;
  kv_scanner_init(&kv_scanner, self->suffix, " ", FALSE);
  GString *formatted_key = scratch_buffers_alloc();

  gchar *formatted_input = _format_input(input, self->suffix);
  kv_scanner_input(&kv_scanner, formatted_input);

  log_msg_make_writable(pmsg, path_options);
  msg_trace("ordered-parser message processing started",
            evt_tag_str ("input", input),
            evt_tag_str ("prefix", self->prefix),
            evt_tag_printf("msg", "%p", *pmsg));

  while (kv_scanner_scan_next(&kv_scanner))
    {
      const gchar *current_key = kv_scanner_get_current_key(&kv_scanner);
      const gchar *current_value = kv_scanner_get_current_value(&kv_scanner);
      log_msg_set_value_by_name(*pmsg,
                                _get_formatted_key(self, current_key, formatted_key),
                                current_value, -1);
    }

  kv_scanner_deinit(&kv_scanner);
  g_free(formatted_input);

  return TRUE;
}

LogPipe *
ordered_parser_clone_method(OrderedParser *dst, OrderedParser *src)
{
  ordered_parser_set_suffix(&dst->super, src->suffix);
  ordered_parser_set_prefix(&dst->super, src->prefix);
  dst->flags = src->flags;

  return &dst->super.super;
}

static LogPipe *
_clone(LogPipe *s)
{
  OrderedParser *self = (OrderedParser *) s;

  OrderedParser *cloned;
  cloned = (OrderedParser *) ordered_parser_new(log_pipe_get_config(s));

  return ordered_parser_clone_method(cloned, self);
}

static void
_free(LogPipe *s)
{
  OrderedParser *self = (OrderedParser *) s;

  g_free(self->prefix);
}

LogParser *
ordered_parser_new(GlobalConfig *cfg)
{
  OrderedParser *self = g_new0(OrderedParser, 1);

  log_parser_init_instance(&self->super, cfg);

  self->super.process = _process;
  self->super.super.clone = _clone;
  self->super.super.free_fn = _free;

  self->suffix = ')';
  self->flags = 0x0000;

  return &self->super;
}
