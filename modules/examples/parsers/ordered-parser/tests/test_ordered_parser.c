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

#include "ordered-parser.h"
#include "apphook.h"
#include "msg_parse_lib.h"
#include "scratch-buffers.h"

#include <criterion/criterion.h>

LogParser *ordered_parser;

static LogMessage *
parse_ordered_list_into_log_message_no_check(const gchar *ordered_list)
{
  LogMessage *msg;
  LogPathOptions path_options = LOG_PATH_OPTIONS_INIT;
  LogParser *cloned_parser;

  cloned_parser = (LogParser *) log_pipe_clone(&ordered_parser->super);
  msg = log_msg_new_empty();
  log_msg_set_value(msg, LM_V_MESSAGE, ordered_list, -1);
  if (!log_parser_process_message(cloned_parser, &msg, &path_options))
    {
      log_msg_unref(msg);
      log_pipe_unref(&cloned_parser->super);
      return NULL;
    }
  log_pipe_unref(&cloned_parser->super);
  return msg;
}

static LogMessage *
parse_ordered_list_into_log_message(const gchar *ordered_list)
{
  LogMessage *msg;

  msg = parse_ordered_list_into_log_message_no_check(ordered_list);
  cr_assert_not_null(msg, "expected ordered-parser success and it returned failure, ordered list=%s", ordered_list);
  return msg;
}

void
setup(void)
{
  app_startup();
  ordered_parser = ordered_parser_new(NULL);
  log_pipe_init((LogPipe *)ordered_parser);
}

void
teardown(void)
{
  log_pipe_deinit((LogPipe *)ordered_parser);
  log_pipe_unref(&ordered_parser->super);
  scratch_buffers_explicit_gc();
  app_shutdown();
}

Test(ordered_parser, basic_default)
{
  LogMessage *msg;

  msg = parse_ordered_list_into_log_message("A) Apple");
  assert_log_message_value_by_name(msg, "A", "Apple");
  log_msg_unref(msg);
}

Test(ordered_parser, letters)
{
  LogMessage *msg;

  ordered_parser_process_flag(ordered_parser, "letters");
  msg = parse_ordered_list_into_log_message("A) Apple B) Banana C) Cherry");
  assert_log_message_value_by_name(msg, "A", "Apple");
  assert_log_message_value_by_name(msg, "B", "Banana");
  assert_log_message_value_by_name(msg, "C", "Cherry");
  log_msg_unref(msg);
}

Test(ordered_parser, numbers)
{
  LogMessage *msg;

  ordered_parser_process_flag(ordered_parser, "numbers");
  msg = parse_ordered_list_into_log_message("1) Apple 2) Banana 3) Cherry");
  assert_log_message_value_by_name(msg, "1", "Apple");
  assert_log_message_value_by_name(msg, "2", "Banana");
  assert_log_message_value_by_name(msg, "3", "Cherry");
  log_msg_unref(msg);
}

Test(ordered_parser, colon_suffix)
{
  LogMessage *msg;

  ordered_parser_set_suffix(ordered_parser, ':');
  msg = parse_ordered_list_into_log_message("A: Apple B: Banana C: Cherry");
  assert_log_message_value_by_name(msg, "A", "Apple");
  assert_log_message_value_by_name(msg, "B", "Banana");
  assert_log_message_value_by_name(msg, "C", "Cherry");
  log_msg_unref(msg);
}

Test(ordered_parser, mixed)
{
  LogMessage *msg;

  ordered_parser_process_flag(ordered_parser, "letters");
  ordered_parser_set_suffix(ordered_parser, ':');
  msg = parse_ordered_list_into_log_message("1: Apple 2: Banana 3: Cherry");
  assert_log_message_value_by_name(msg, "1", "Apple");
  assert_log_message_value_by_name(msg, "2", "Banana");
  assert_log_message_value_by_name(msg, "3", "Cherry");
  log_msg_unref(msg);
}

Test(ordered_parser, varying_spaces)
{
  LogMessage *msg;

  ordered_parser_process_flag(ordered_parser, "letters");
  ordered_parser_set_suffix(ordered_parser, '=');
  msg = parse_ordered_list_into_log_message("A=    Apple    B    =Banana C    =    Cherry");
  assert_log_message_value_by_name(msg, "A", "Apple");
  assert_log_message_value_by_name(msg, "B", "Banana");
  assert_log_message_value_by_name(msg, "C", "Cherry");
  log_msg_unref(msg);
}

TestSuite(ordered_parser, .init = setup, .fini = teardown);
