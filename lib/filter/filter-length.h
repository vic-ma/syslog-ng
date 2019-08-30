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

#ifndef FILTER_LENGTH_H_INCLUDED
#define FILTER_LENGTH_H_INCLUDED

#include "filter/filter-expr.h"

FilterExprNode *filter_len_lt_new(int length);
FilterExprNode *filter_len_le_new(int length);
FilterExprNode *filter_len_gt_new(int length);
FilterExprNode *filter_len_ge_new(int length);
FilterExprNode *filter_len_eq_new(int length);
FilterExprNode *filter_len_ne_new(int length);

FilterExprNode *filter_len_gtlt_new(int min, int max);
FilterExprNode *filter_len_gtle_new(int min, int max);
FilterExprNode *filter_len_gelt_new(int min, int max);
FilterExprNode *filter_len_gele_new(int min, int max);

#endif
