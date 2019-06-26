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
#include <unistd.h>
#include <fcntl.h>

#include "static-file-reader.h"
#include "poll-fd-events.h"
#include "logproto/logproto-multiline-server.h"
#include "transport/transport-file.h"

void
static_file_reader_options_defaults(StaticFileReaderOptions *options)
{
  log_source_options_defaults(&options->reader_options.super);
}

void
static_file_reader_options_init(StaticFileReaderOptions *options, GlobalConfig *cfg, const gchar *group)
{
  log_reader_options_init(&options->reader_options, cfg, group);
}

static LogProtoServer *
_construct_proto(StaticFileReader *self)
{
  LogProtoServerOptions *proto_options = &self->options->reader_options.proto_options.super;

  LogTransport *transport = log_transport_file_new(self->file);

  proto_options->position_tracking_enabled = FALSE;

  return log_proto_text_server_new(transport, proto_options);
}

static void
_setup_logreader(LogPipe *s, PollEvents *poll_events, LogProtoServer *proto)
{
  StaticFileReader *self = (StaticFileReader *) s;

  self->reader = log_reader_new(log_pipe_get_config(s));
  log_reader_open(self->reader, proto, poll_events);
  log_reader_set_options(self->reader,
                         s,
                         &self->options->reader_options,
                         self->owner->super.id,
                         self->pathname->str);

  log_reader_set_immediate_check(self->reader);
  log_pipe_append((LogPipe *) self->reader, s);
}

static gboolean
static_file_reader_init(LogPipe *s)
{
  StaticFileReader *self = (StaticFileReader *) s;

  if((self->file = open(self->pathname->str, O_RDONLY)) == -1)
    {
      msg_error("Error: file not found", evt_tag_str("pathname", self->pathname->str));
      return FALSE;
    }

  PollEvents *poll_events = poll_fd_events_new(self->file);

  LogProtoServer *proto = _construct_proto(self);

  _setup_logreader(s, poll_events, proto);

  if (!log_pipe_init(((LogPipe *) self->reader)))
    {
      msg_error("Error initializing log_reader, closing file",
                evt_tag_int("fd", self->file));
      log_pipe_unref((LogPipe *) self->reader);
      self->reader = NULL;
      close(self->file);
      return FALSE;
    }
  return TRUE;
}

static gboolean
static_file_reader_deinit(LogPipe *s)
{
  StaticFileReader *self = (StaticFileReader *) s;
}

static void
static_file_reader_free(LogPipe *s)
{
  StaticFileReader *self = (StaticFileReader *) s;
}

StaticFileReader *
static_file_reader_new(const gchar *pathname, StaticFileReaderOptions *options, LogSrcDriver *owner,
                       GlobalConfig *cfg)
{
  StaticFileReader *self = g_new0(StaticFileReader, 1);

  log_pipe_init_instance (&self->super, cfg);
  self->super.init = static_file_reader_init;
  self->super.deinit = static_file_reader_deinit;
  self->super.free_fn = static_file_reader_free;

  self->pathname = g_string_new(pathname);
  self->options = options;
  self->owner = owner;

  return self;
}
