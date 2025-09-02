/*
 * MD4C: Markdown parser for C
 * (http://github.com/mity/md4c)
 *
 * Copyright (c) 2016-2024 Martin Mitáš
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "md4c-html.h"
#include "cmdline.h"


/* Global options. */
static unsigned parser_flags = 0;
#ifndef MD4C_USE_ASCII
    static unsigned renderer_flags = MD_HTML_FLAG_DEBUG | MD_HTML_FLAG_SKIP_UTF8_BOM;
#else
    static unsigned renderer_flags = MD_HTML_FLAG_DEBUG;
#endif
static int want_fullhtml = 0;
static int want_xhtml = 0;
static int want_stat = 0;
static int want_replay_fuzz = 0;

static const char* html_title = NULL;
static const char* css_path = NULL;

static const char* input_path = NULL;
static const char* output_path = NULL;


static const CMDLINE_OPTION cmdline_options[] = {
    { 'o', "output",                        'o', CMDLINE_OPTFLAG_REQUIREDARG },
    { 'f', "full-html",                     'f', 0 },
    { 'x', "xhtml",                         'x', 0 },
    { 's', "stat",                          's', 0 },
    { 'h', "help",                          'h', 0 },
    { 'v', "version",                       'v', 0 },

    {  0,  "html-title",                    '1', CMDLINE_OPTFLAG_REQUIREDARG },
    {  0,  "html-css",                      '2', CMDLINE_OPTFLAG_REQUIREDARG },

    {  0,  "commonmark",                    'c', 0 },
    {  0,  "github",                        'g', 0 },

    {  0,  "fcollapse-whitespace",          'W', 0 },
    {  0,  "flatex-math",                   'L', 0 },
    {  0,  "fpermissive-atx-headers",       'A', 0 },
    {  0,  "fpermissive-autolinks",         'V', 0 },
    {  0,  "fhard-soft-breaks",             'B', 0 },
    {  0,  "fpermissive-email-autolinks",   '@', 0 },
    {  0,  "fpermissive-url-autolinks",     'U', 0 },
    {  0,  "fpermissive-www-autolinks",     '.', 0 },
    {  0,  "fstrikethrough",                'S', 0 },
    {  0,  "ftables",                       'T', 0 },
    {  0,  "ftasklists",                    'X', 0 },
    {  0,  "funderline",                    '_', 0 },
    {  0,  "fverbatim-entities",            'E', 0 },
    {  0,  "fwiki-links",                   'K', 0 },

    {  0,  "fno-html-blocks",               'F', 0 },
    {  0,  "fno-html-spans",                'G', 0 },
    {  0,  "fno-html",                      'H', 0 },
    {  0,  "fno-indented-code",             'I', 0 },

    /* Undocumented option for replaying test cases from fuzzers. */
    {  0,  "replay-fuzz",                   'r', 0 },

    {  0,  NULL,                             0,  0 }
};


/*********************************
 ***  Simple grow-able buffer  ***
 *********************************/

/* We render to a memory buffer instead of directly outputting the rendered
 * documents, as this allows using this utility for evaluating performance
 * of MD4C (--stat option). This allows us to measure just time of the parser,
 * without the I/O.
 */

struct membuffer {
    char* data;
    size_t asize;
    size_t size;
};

void membuf_init(struct membuffer* buf, MD_SIZE new_asize);

void membuf_fini(struct membuffer* buf);

void membuf_grow(struct membuffer* buf, size_t new_asize);

void membuf_append(struct membuffer* buf, const char* data, MD_SIZE size);


/**********************
 ***  Main program  ***
 **********************/

void process_output(const MD_CHAR* text, MD_SIZE size, void* userdata);

char* process_file(const char* in_path, FILE* in, FILE* out);

void usage(void);

void version(void);

int cmdline_callback(int opt, char const* value, void* data);

char* process_string(const char* input);

int initMdParser(int argc, char** argv);
