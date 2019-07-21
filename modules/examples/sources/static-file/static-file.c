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
#include "static-file-reader.h"

static gboolean
_open_file(LogThreadedFetcherDriver *s)
{
  StaticFileReader *self = (StaticFileReader *) s;
  return stf_open(self->reader, self->pathname);
}

static void
_close_file(LogThreadedFetcherDriver *s)
{
  StaticFileReader *self = (StaticFileReader *) s;
  stf_close(self->reader);
}

static LogThreadedFetchResult
_fetch_line(LogThreadedFetcherDriver *self)
{
  StaticFileReader *self = (StaticFileReader *) s;
}

LogDriver *
static_file_sd_new(GlobalConfig *cfg)
{
  StaticFileDriver *self = g_new0(StaticFileDriver, 1);
  log_threaded_fetcher_Driver_init_instance(&self->super, cfg);

  self->super.connect = _open_file;
  self->super.disconnect = _close_file;
  self->super.fetch = _fetch_line;
}
