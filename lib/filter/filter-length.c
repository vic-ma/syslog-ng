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

typedef struct _FilterLengthRange
{
  FilterExprNode super;
  gint min;
  gint max;
} FilterLengthRange;

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
      result = (gint) strlen(message_part) comp_op self->length;                          \
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

#define IMPLEMENT_FILTER_LEN_RANGE(comp_name, comp_op_1, comp_op_2)                       \
    static gboolean                                                                       \
    filter_len_ ## comp_name ## _eval(FilterExprNode *s, LogMessage **msgs, gint num_msg) \
    {                                                                                     \
      FilterLengthRange *self = (FilterLengthRange *) s;                                  \
      gboolean result;                                                                    \
                                                                                          \
      LogMessage *msg = msgs[num_msg - 1];                                                \
      const gchar *message_part = log_msg_get_value(msg, LM_V_MESSAGE, NULL);             \
                                                                                          \
      result = ((gint) strlen(message_part) comp_op_1 self->min) &&                       \
               ((gint) strlen(message_part) comp_op_2 self->max);                         \
      return result ^ s->comp;                                                            \
    }                                                                                     \
                                                                                          \
    FilterExprNode *                                                                      \
    filter_len_ ## comp_name ## _new(gint min, gint max)                                  \
    {                                                                                     \
      FilterLengthRange *self = g_new0(FilterLengthRange, 1);                             \
      filter_expr_node_init_instance(&self->super);                                       \
      self->super.eval = filter_len_ ## comp_name ## _eval;                               \
      self->min = min;                                                                    \
      self->max = max;                                                                    \
      return &self->super;                                                                \
    }

IMPLEMENT_FILTER_LEN_SINGLE(lt, <)
IMPLEMENT_FILTER_LEN_SINGLE(le, <=)
IMPLEMENT_FILTER_LEN_SINGLE(gt, >)
IMPLEMENT_FILTER_LEN_SINGLE(ge, >=)
IMPLEMENT_FILTER_LEN_SINGLE(eq, ==)
IMPLEMENT_FILTER_LEN_SINGLE(ne, !=)

IMPLEMENT_FILTER_LEN_RANGE(gtlt, >, <)
IMPLEMENT_FILTER_LEN_RANGE(gtle, >, <=)
IMPLEMENT_FILTER_LEN_RANGE(gelt, >=, <)
IMPLEMENT_FILTER_LEN_RANGE(gele, >=, <=)
