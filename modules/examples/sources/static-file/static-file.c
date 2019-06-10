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
#include "driver.h"

#define MAX_LINE_LENGTH 2000

static void read_lines(StaticFileSourceDriver *self)
{
  gchar *line = g_malloc(MAX_LINE_LENGTH);
  while (fgets(line, MAX_LINE_LENGTH, self->file) != NULL)
    {
      printf("%s", line);
    }
  g_free(line);
}

static gboolean
static_file_sd_init(LogPipe *s)
{
  printf("INIT\n");

  StaticFileSourceDriver *self = (StaticFileSourceDriver *) s;
  self->file = fopen(self->filename->str, "r");

  /* Initialize stats */
  if (!log_src_driver_init_method(s))
    return FALSE;

  read_lines(self);
  return TRUE;
}

static void
static_file_sd_free(LogPipe *s)
{
  printf("FREE\n");
  StaticFileSourceDriver *self = (StaticFileSourceDriver *) s;

  g_string_free(self->filename, TRUE);
  fclose(self->file);
  log_src_driver_free(s);
}

LogDriver *
static_file_sd_new(gchar *filename, GlobalConfig *cfg)
{
  printf("NEW\n");
  StaticFileSourceDriver *self = g_new0(StaticFileSourceDriver, 1);

  /* Set default values for struct members */
  log_src_driver_init_instance(&self->super, cfg);

  self->super.super.super.init = static_file_sd_init;
  self->super.super.super.free_fn = static_file_sd_free;
  self->filename = g_string_new(filename);

  return &self->super.super;
}
