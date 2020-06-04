/**
 * Copyright (c) 2012 ooxi/xml.c
 *     https://github.com/ooxi/xml.c
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software in a
 *     product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 * 
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 */
#ifndef HEADER_XML
#define HEADER_XML


/**
 * Includes
 */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


void put_head(char *head);

// <tag>
void tag_start(char *tag);
// </tag>
void tag_end(char *tag);
// <tag>content</tag>
void tag_content(char *tag, char *content);
// <tag "name"="rinetd" >
void tag_name(char *tag, char *name);
// <tag "value"="diao" >
void tag_value(char *tag, char *value);
// <tag "value"=1 >
void tag_value_num(char *tag, long value);

void tag_result_filename_flag(char *tag, char * filename, char flag);
#ifdef __cplusplus
}
#endif

#endif

