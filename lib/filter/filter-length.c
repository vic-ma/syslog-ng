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

#include "filter-length.h"

typedef struct _FilterLength
{
  FilterExprNode super;
  int length;
} FilterLength;

static gboolean
filter_length_eval(FilterExprNode *s, LogMessage **msgs, gint num_msg)
{
  FilterLength *self = (FilterLength *) s;
  gboolean result;

  LogMessage *msg = msgs[num_msg - 1];
  const gchar *msg_part = log_msg_get_value(msg, LM_V_MESSAGE, NULL);

  result =  strlen(msg_part) <= self->length;
  return result ^ s->comp;
}

FilterExprNode *
filter_length_new(int length)
{
  FilterLength *self = g_new0(FilterLength, 1);
  filter_expr_node_init_instance(&self->super);
  self->super.eval = filter_length_eval;
  self->length = length;
  return &self->super;
}
