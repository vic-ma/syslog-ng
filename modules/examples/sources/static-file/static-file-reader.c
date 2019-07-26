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

StaticFileReader *
sfr_new(void)
{
  return g_new0(StaticFileReader, 1);
}

gboolean
sfr_open(StaticFileReader *self, gchar *pathname)
{
  self->file = fopen(pathname, "r");
  return self->file != NULL;
}

GString *
sfr_nextline(StaticFileReader *self, gsize maxlen)
{
  gchar *temp_buf = g_malloc(maxlen);
  if (!fgets(temp_buf, maxlen, self->file))
    {
      g_free(temp_buf);
      return NULL;
    }

  GString *line = g_string_new(temp_buf);
  g_free(temp_buf);
  return line;
}

void
sfr_close(StaticFileReader *self)
{
  fclose(self->file);
}

void
sfr_free(StaticFileReader *self)
{
  g_free(self);
}
