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

#ifndef STATIC_FILE_READER_H
#define STATIC_FILE_READER_H

#include "logsource.h"
#include "logreader.h"
#include "driver.h"

typedef struct _StaticFileReaderOptions
{
    LogReaderOptions reader_options;
} StaticFileReaderOptions;

typedef struct _StaticFileReader
{
    LogPipe super;
    LogReader *reader;
    StaticFileReaderOptions *options;
    LogSrcDriver *owner;
    GString *pathname;
    gint file;
} StaticFileReader;

void static_file_reader_options_defaults(StaticFileReaderOptions *options);
void static_file_reader_options_init(StaticFileReaderOptions *options, GlobalConfig *cfg, const gchar *group);

StaticFileReader *static_file_reader_new(const gchar *pathname, StaticFileReaderOptions *options,
                                        LogSrcDriver *owner, GlobalConfig *cfg);

#endif
