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


/***************************
 * SINGLE COMPARISON TESTS *
 ***************************/


typedef struct _FilterParamLengthSingle
{
  const gchar *msg;
  gint length;
  gboolean expected_result;
} FilterParamLengthSingle;

ParameterizedTestParameters(filter, test_filter_len_lt)
{
  static FilterParamLengthSingle test_data_list[] =
  {
    {.msg = "",                                                 .length = 0,   .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: ",                             .length = -1,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 26,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 27,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 28,  .expected_result = TRUE},
  };

  return cr_make_param_array(FilterParamLengthSingle, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLengthSingle *param, filter, test_filter_len_lt)
{
  FilterExprNode *filter = filter_len_lt_new(param->length);
  testcase(param->msg, filter, param->expected_result);
}

ParameterizedTestParameters(filter, test_filter_len_le)
{
  static FilterParamLengthSingle test_data_list[] =
  {
    {.msg = "",                                                 .length = 0,   .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: ",                             .length = -1,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 26,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 27,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 28,  .expected_result = TRUE},
  };

  return cr_make_param_array(FilterParamLengthSingle, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLengthSingle *param, filter, test_filter_len_le)
{
  FilterExprNode *filter = filter_len_le_new(param->length);
  testcase(param->msg, filter, param->expected_result);
}


ParameterizedTestParameters(filter, test_filter_len_gt)
{
  static FilterParamLengthSingle test_data_list[] =
  {
    {.msg = "",                                                 .length = 0,   .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: ",                             .length = -1,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 26,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 27,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 28,  .expected_result = FALSE},
  };

  return cr_make_param_array(FilterParamLengthSingle, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLengthSingle *param, filter, test_filter_len_gt)
{
  FilterExprNode *filter = filter_len_gt_new(param->length);
  testcase(param->msg, filter, param->expected_result);
}

ParameterizedTestParameters(filter, test_filter_len_ge)
{
  static FilterParamLengthSingle test_data_list[] =
  {
    {.msg = "",                                                 .length = 0,   .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: ",                             .length = -1,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 26,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 27,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 28,  .expected_result = FALSE},
  };

  return cr_make_param_array(FilterParamLengthSingle, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLengthSingle *param, filter, test_filter_len_ge)
{
  FilterExprNode *filter = filter_len_ge_new(param->length);
  testcase(param->msg, filter, param->expected_result);
}

ParameterizedTestParameters(filter, test_filter_len_eq)
{
  static FilterParamLengthSingle test_data_list[] =
  {
    {.msg = "",                                                 .length = 0,   .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: ",                             .length = -1,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 26,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 27,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 28,  .expected_result = FALSE},
  };

  return cr_make_param_array(FilterParamLengthSingle, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLengthSingle *param, filter, test_filter_len_eq)
{
  FilterExprNode *filter = filter_len_eq_new(param->length);
  testcase(param->msg, filter, param->expected_result);
}
ParameterizedTestParameters(filter, test_filter_len_ne)
{
  static FilterParamLengthSingle test_data_list[] =
  {
    {.msg = "",                                                 .length = 0,   .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: ",                             .length = -1,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 26,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 27,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .length = 28,  .expected_result = TRUE},
  };

  return cr_make_param_array(FilterParamLengthSingle, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLengthSingle *param, filter, test_filter_len_ne)
{
  FilterExprNode *filter = filter_len_ne_new(param->length);
  testcase(param->msg, filter, param->expected_result);
}

/**************************
 * RANGE COMPARISON TESTS *
 **************************/

typedef struct _FilterParamLengthRange
{
  const gchar *msg;
  gint min;
  gint max;
  gboolean expected_result;
} FilterParamLengthRange;

ParameterizedTestParameters(filter, test_filter_len_gtlt)
{
  static FilterParamLengthRange test_data_list[] =
  {
    {.msg = "",                                                 .min = 0,  .max = 0,   .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: ",                             .min = -1,  .max = 1,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: ",                             .min = 1,  .max = -1,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 26, .max = 28,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 27, .max = 27,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 26, .max = 27,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 27, .max = 28,  .expected_result = FALSE},
  };

  return cr_make_param_array(FilterParamLengthRange, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLengthRange *param, filter, test_filter_len_gtlt)
{
  FilterExprNode *filter = filter_len_gtlt_new(param->min, param->max);
  testcase(param->msg, filter, param->expected_result);
}

ParameterizedTestParameters(filter, test_filter_len_gtle)
{
  static FilterParamLengthRange test_data_list[] =
  {
    {.msg = "",                                                 .min = 0,  .max = 0,   .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: ",                             .min = -1,  .max = 1,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: ",                             .min = 1,  .max = -1,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 26, .max = 28,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 27, .max = 27,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 26, .max = 27,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 27, .max = 28,  .expected_result = FALSE},
  };

  return cr_make_param_array(FilterParamLengthRange, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLengthRange *param, filter, test_filter_len_gtle)
{
  FilterExprNode *filter = filter_len_gtle_new(param->min, param->max);
  testcase(param->msg, filter, param->expected_result);
}

ParameterizedTestParameters(filter, test_filter_len_gelt)
{
  static FilterParamLengthRange test_data_list[] =
  {
    {.msg = "",                                                 .min = 0,  .max = 0,   .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: ",                             .min = -1,  .max = 1,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: ",                             .min = 1,  .max = -1,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 26, .max = 28,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 27, .max = 27,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 26, .max = 27,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 27, .max = 28,  .expected_result = TRUE},
  };

  return cr_make_param_array(FilterParamLengthRange, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLengthRange *param, filter, test_filter_len_gelt)
{
  FilterExprNode *filter = filter_len_gelt_new(param->min, param->max);
  testcase(param->msg, filter, param->expected_result);
}

ParameterizedTestParameters(filter, test_filter_len_gele)
{
  static FilterParamLengthRange test_data_list[] =
  {
    {.msg = "",                                                 .min = 0,  .max = 0,   .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: ",                             .min = -1,  .max = 1,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: ",                             .min = 1,  .max = -1,  .expected_result = FALSE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 26, .max = 28,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 27, .max = 27,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 26, .max = 27,  .expected_result = TRUE},
    {.msg = "<15> openvpn[2499]: PTHREAD support initialized",  .min = 27, .max = 28,  .expected_result = TRUE},
  };

  return cr_make_param_array(FilterParamLengthRange, test_data_list,  G_N_ELEMENTS(test_data_list));
}

ParameterizedTest(FilterParamLengthRange *param, filter, test_filter_len_gele)
{
  FilterExprNode *filter = filter_len_gele_new(param->min, param->max);
  testcase(param->msg, filter, param->expected_result);
}
