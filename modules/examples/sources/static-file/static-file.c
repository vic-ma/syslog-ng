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

static gboolean
static_file_sd_init(LogPipe *s)
{
  if (!log_src_driver_init_method(s))
    return FALSE;

  StaticFileSourceDriver *self = (StaticFileSourceDriver *) s;
  GlobalConfig *cfg = log_pipe_get_config(s);

  self->reader = static_file_reader_new(self->filename->str, cfg);

  log_source_options_init(&self->reader_options.super, cfg, self->super.super.group);
  log_source_set_options(&self->reader->super, &self->reader_options.super, self->super.super.id,
                         NULL, FALSE, FALSE, self->super.super.super.expr_node);

  log_pipe_append(&self->reader->super.super, s);

  if (!log_pipe_init(&self->reader->super.super))
    {
      log_pipe_unref(&self->reader->super.super);
      self->reader = NULL;
      return FALSE;
    }
  return TRUE;
}

static gboolean
static_file_sd_deinit(LogPipe *s)
{
  StaticFileSourceDriver *self = (StaticFileSourceDriver *) s;

  log_pipe_deinit(&self->reader->super.super);

  return log_src_driver_deinit_method(s);
}

static void
static_file_sd_free(LogPipe *s)
{
  printf("FREE\n");
  StaticFileSourceDriver *self = (StaticFileSourceDriver *) s;

  log_pipe_unref(&self->reader->super.super);
  g_string_free(self->filename, TRUE);
  log_source_options_destroy(&self->reader_options.super);
  log_src_driver_free(s);
}

LogDriver *
static_file_sd_new(gchar *filename, GlobalConfig *cfg)
{
  StaticFileSourceDriver *self = g_new0(StaticFileSourceDriver, 1);

  log_src_driver_init_instance(&self->super, cfg);
  log_source_options_defaults(&self->reader_options.super);

  self->super.super.super.init = static_file_sd_init;
  self->super.super.super.deinit = static_file_sd_deinit;
  self->super.super.super.free_fn = static_file_sd_free;
  self->filename = g_string_new(filename);

  return &self->super.super;
}
