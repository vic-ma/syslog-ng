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
#include "cfg-parser.h"
#include "ordered-parser-grammar.h"

extern int ordered_parser_debug;

int ordered_parser_parse(CfgLexer *lexer, LogParser **instance, gpointer arg);

static CfgLexerKeyword ordered_parser_keywords[] =
{
  { "example_ordered_parser", KW_ORDERED_PARSER },
  { "suffix",                 KW_SUFFIX },
  { "prefix",                 KW_PREFIX },
  { NULL }
};

CfgParser ordered_parser_parser =
{
#if SYSLOG_NG_ENABLE_DEBUG
  .debug_flag = &ordered_parser_debug,
#endif
  .name = "ordered-parser",
  .keywords = ordered_parser_keywords,
  .parse = (gint (*)(CfgLexer *, gpointer *, gpointer)) ordered_parser_parse,
  .cleanup = (void (*)(gpointer)) log_pipe_unref,
};

CFG_PARSER_IMPLEMENT_LEXER_BINDING(ordered_parser_, LogParser **)
