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

#include "filter-length-parser.h"
#include "filter-length-grammar.h"
#include "filter-length.h"

extern int filter_length_debug;
int filter_length_parse(CfgLexer *lexer, FilterExprNode **node, gpointer arg);

static CfgLexerKeyword filter_length_keywords[] =
{
  { "example_filter_length", KW_FILTER_LENGTH  },
  { "len_lt", KW_LEN_LT },
  { NULL }
};

CfgParser filter_length_parser =
{
#if SYSLOG_NG_ENABLE_DEBUG
  .debug_flag = &filter_length_debug,
#endif
  .name = "filter-length",
  .context = LL_CONTEXT_FILTER,
  .keywords = filter_length_keywords,
  .parse = (gint (*)(CfgLexer *, gpointer *, gpointer)) filter_length_parse,
};

CFG_PARSER_IMPLEMENT_LEXER_BINDING(filter_length_, FilterExprNode **)
