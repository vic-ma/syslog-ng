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
stf_new(void)
{
  return g_new0(StaticFileReader, 1);
}

gboolean
stf_open(StaticFileReader *self, gchar *pathname)
{
  self->file = fopen(pathname, "r");
  return self->file != NULL;
}

GString *
stf_nextline(StaticFileReader *self, gsize maxlen)
{
  GString *line = g_string_sized_new(maxlen);
  if (!fgets(line->str, maxlen, self->file))
    {
      g_string_free(line, TRUE);
      return NULL;
    }
  return line;
}

void
stf_close(StaticFileReader *self)
{
  fclose(self->file);
}

void
stf_free(StaticFileReader *self)
{
  g_free(self);
}
