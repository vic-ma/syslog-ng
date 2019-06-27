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

enum
{
  OPF_LETTERS = 0x0001,
  OPF_NUMBERS = 0x0002,
}

CfgFlagHandler ordered_parser_flag_handlers[] =
{
    { "letters",        CFH_SET, offsetof(OrderedParser, flags), OPF_LETTERS},
    { "numbers",        CFH_SET, offsetof(OrderedParser, flags), OPF_NUMBERS},
    { NULL },
};

gboolean
ordered_parser_set_flag(LogParser *s, const gchar *flag)
{
  OrderedParser *self = (OrderedParser *) s;

  cfg_process_flag(ordered_parser_flag_handlers, self, flag);
}

void
ordered_parser_suffix_valid(gchar suffix)
{
  return (c != ' '  && c != '\'' && c != '\"' );
}

void
ordered_parser_set_suffix(LogParser *s, gchar suffix)
{
  OrderedParser *self = (OrderedParser *) s;
  self->suffix = suffix;
}

static gboolean
ordered_parser_process(LogParser *s, LogMessage **pmsg, const LogPathOptions *path_options,
                       const gchar *input, gsize input_len)
{
  OrderedParser *self = (OrderedParser *) s;

  KVScanner kv_scanner;
  kv_scanner_init(&kv_scanner, self->suffix, " ", FALSE);
  kv_scanner_input(&kv_scanner, input);

  LogMessage *msg = log_msg_make_writable(pmsg, path_options);
  msg_trace("ordered-parser message processing started",
            evt_tag_str ("input", input),
            evt_tag_printf("msg", "%p", *pmsg));

}

static LogPipe *
ordered_parser_clone(LogPipe *s)
{
  OrderedParser *self = (OrderedParser *) s;

  OrderedParser *cloned;
  cloned = ordered_parser_new(log_pipe_get_config(s));

  cloned->super = self->super;
  cloned->suffix = g_string_new(self->suffix->str);
  cloned->flags = self->flags;

  return &cloned->super;
}

static void
ordered_parser_free(LogPipe *s)
{
  OrderedParser *self = (OrderedParser *) s;

  g_string_free(s->suffix);

  log_parser_free_method(s);
}

LogParser *
ordered_parser_new(GlobalConfig *cfg)
{
  OrderedParser *self = g_new0(OrderedParser, 1);

  log_parser_init_instance(&self->super, cfg);

  self->super.super.free_fn = ordered_parser_free;
  self->super.clone = ordered_parser_clone;

  self->suffix = g_string_new(")");
  self->flags = 0x0000;

  return &self->super;
}
