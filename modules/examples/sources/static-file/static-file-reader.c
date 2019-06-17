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

void
read_file(StaticFileReader *self)
{
  if (!log_source_free_to_send(&self->super))
    return;

  gchar *line = g_malloc(MAX_LINE_LENGTH);
  LogMessage *msg = log_msg_new_empty();

  while (fgets(line, MAX_LINE_LENGTH, self->file) != NULL)
    {
      log_msg_set_value(msg, LM_V_MESSAGE, line, -1);
      printf("sending\n");
      log_source_post(&self->super, msg);
      printf("ending\n");
    }

  g_free(line);
}

StaticFileReader *
static_file_reader_new(const gchar *filename, GlobalConfig *cfg)
{
  printf("READER NEW\n");

  StaticFileReader *self = g_new0(StaticFileReader, 1);
  self->file = fopen(filename, "r");

  /* Set defaults for struct members */
  log_source_init_instance(&self->super, cfg);

  return self;
}
