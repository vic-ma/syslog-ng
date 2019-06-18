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

#include "static-file-reader.h"

#define MAX_LINE_LENGTH 2000

static void
start_timer(StaticFileReader *self)
{
  iv_validate_now();
  self->timer.expires = iv_now;
  iv_timer_register(&self->timer);
}

static void
stop_timer(StaticFileReader *self)
{
  if (iv_timer_registered(&self->timer))
    iv_timer_unregister(&self->timer);
}


static void
timer_expired(void *cookie)
{
  StaticFileReader *self = (StaticFileReader *) cookie;
  read_file(self);
}

void
read_file(StaticFileReader *self)
{
  if (!log_source_free_to_send(&self->super))
    return;

  gchar *line = g_malloc(MAX_LINE_LENGTH);

  while (fgets(line, MAX_LINE_LENGTH, self->file) != NULL)
    {
      LogMessage *msg = log_msg_new_empty();
      log_msg_set_value(msg, LM_V_MESSAGE, line, -1);
      log_source_post(&self->super, msg);
    }

  g_free(line);
}

static gboolean
static_file_reader_init(LogPipe *s)
{
  StaticFileReader *self = (StaticFileReader *) s;

  self->file = fopen(self->filename->str, "r");
  if (!self->file)
    {
      msg_error("Error: file not found", evt_tag_str("filename", self->filename->str));
      return FALSE;
    }

  if (!log_source_init(s))
    return FALSE;

  start_timer(self);

  return TRUE;
}

static gboolean
static_file_reader_deinit(LogPipe *s)
{
  StaticFileReader *self = (StaticFileReader *) s;
  stop_timer(self);
  return log_source_deinit(s);
}

static void
static_file_reader_free(LogPipe *s)
{
  StaticFileReader *self = (StaticFileReader *) s;
  g_string_free(self->filename, TRUE);
  fclose(self->file);
  log_source_free(s);
}

StaticFileReader *
static_file_reader_new(const gchar *filename, GlobalConfig *cfg)
{
  StaticFileReader *self = g_new0(StaticFileReader, 1);

  self->filename = g_string_new(filename);

  log_source_init_instance(&self->super, cfg);

  self->super.super.init = static_file_reader_init;
  self->super.super.deinit = static_file_reader_deinit;
  self->super.super.free_fn = static_file_reader_free;

  IV_TIMER_INIT(&self->timer);
  self->timer.cookie = self;
  self->timer.handler = timer_expired;

  return self;
}
