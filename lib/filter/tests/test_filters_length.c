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

#include "filter/filter-expr.h"
#include "filter/filter-length.h"
#include "cfg.h"
#include "test_filters_common.h"

#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

TestSuite(filter, .init = setup, .fini = teardown);

typedef struct _FilterParamLength
{
  const gchar *msg;
  int length;
  gboolean expected_result;
} FilterParamLength;

ParameterizedTestParameters(filter, test_filter_length)
{
  static FilterParamLength test_data_list[] =
    {
        {.msg = "",                                                 .length = 0,  .expected_result = TRUE },
        {.msg = "<15> openvpn[2499]: ",                             .length = 0,  .expected_result = TRUE },
        {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 27, .expected_result = TRUE },
        {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 26, .expected_result = FALSE},
        {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 0,  .expected_result = FALSE},
    };

  return cr_make_param_array(FilterParamLength, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLength *param, filter, test_filter_length)
{
  FilterExprNode *filter = filter_length_new(param->length);
  testcase(param->msg, filter, param->expected_result);
}
