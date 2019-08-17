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

typedef struct _FilterLengthSingle
{
  FilterExprNode super;
  gint length;
} FilterLengthSingle;

#define IMPLEMENT_FILTER_LEN_SINGLE(comp_name, comp_op)                                   \
    static gboolean                                                                       \
    filter_len_ ## comp_name ## _eval(FilterExprNode *s, LogMessage **msgs, gint num_msg) \
    {                                                                                     \
      FilterLengthSingle *self = (FilterLengthSingle *) s;                                \
      gboolean result;                                                                    \
                                                                                          \
      LogMessage *msg = msgs[num_msg - 1];                                                \
      const gchar *message_part = log_msg_get_value(msg, LM_V_MESSAGE, NULL);             \
                                                                                          \
      result = strlen(message_part) comp_op self->length;                                 \
      return result ^ s->comp;                                                            \
    }                                                                                     \
                                                                                          \
    FilterExprNode *                                                                      \
    filter_len_ ## comp_name ## _new(gint length)                                         \
    {                                                                                     \
      FilterLengthSingle *self = g_new0(FilterLengthSingle, 1);                           \
      filter_expr_node_init_instance(&self->super);                                       \
      self->super.eval = filter_len_ ## comp_name ## _eval;                               \
      self->length = length;                                                              \
      return &self->super;                                                                \
    }

IMPLEMENT_FILTER_LEN_SINGLE(lt, <)
