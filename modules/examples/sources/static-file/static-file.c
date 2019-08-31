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

#include "static-file.h"
#include "logmsg/logmsg.h"
#include "messages.h"

static const gchar *
_format_stats_instance(LogThreadedSourceDriver *s)
{
  StaticFileSourceDriver *self = (StaticFileSourceDriver *) s;
  static gchar persist_name[1024];

  if (s->super.super.super.persist_name)
    g_snprintf(persist_name, sizeof(persist_name), "static-file,%s", s->super.super.super.persist_name);
  else
    g_snprintf(persist_name, sizeof(persist_name), "static-file,%s", self->pathname);

  return persist_name;
}

static gboolean
_open_file(LogThreadedFetcherDriver *s)
{
  StaticFileSourceDriver *self = (StaticFileSourceDriver *) s;
  return sfr_open(self->reader, self->pathname);
}

static LogThreadedFetchResult
_fetch_line(LogThreadedFetcherDriver *s)
{
  StaticFileSourceDriver *self = (StaticFileSourceDriver *) s;

  GString *line = sfr_nextline(self->reader, SF_MAXLEN);

  if (!line)
    {
      LogThreadedFetchResult result = { THREADED_FETCH_NOT_CONNECTED, NULL };
      return result;
    }
  g_string_truncate(line, line->len-1);

  LogMessage *msg = log_msg_new_empty();
  log_msg_set_value(msg, LM_V_MESSAGE, line->str, line->len);
  LogThreadedFetchResult result = { THREADED_FETCH_SUCCESS, msg };
  return result;
}

static void
_close_file(LogThreadedFetcherDriver *s)
{
  StaticFileSourceDriver *self = (StaticFileSourceDriver *) s;
  sfr_close(self->reader);
}

static void
_free(LogPipe *s)
{
  StaticFileSourceDriver *self = (StaticFileSourceDriver *) s;

  g_free(self->pathname);
  sfr_free(self->reader);

  log_threaded_fetcher_driver_free_method(s);
}

LogDriver *
static_file_sd_new(gchar *pathname, GlobalConfig *cfg)
{
  StaticFileSourceDriver *self = g_new0(StaticFileSourceDriver, 1);
  log_threaded_fetcher_driver_init_instance(&self->super, cfg);

  self->super.super.super.super.super.free_fn = _free;
  self->super.super.format_stats_instance = _format_stats_instance;

  self->super.connect = _open_file;
  self->super.disconnect = _close_file;
  self->super.fetch = _fetch_line;

  self->reader = sfr_new();
  self->pathname = g_strdup(pathname);

  return &self->super.super.super.super;
}
