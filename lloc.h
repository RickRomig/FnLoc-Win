/*
 * FILE
 *      lloc.h -- header file for loc.c
 * NAME
 *      Copyright (C) 2019  Richard Romig
 * EMAIL
 *      rick.romig@gmail.com
 * DATE
 *      25 January 2019
 * DESCRIPTION:
 * Header file for lloc.c
 * Declares constants, global variables, enumberated types and functions.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * 25 January 2019
*/

#define BUFF_LEN 128

/* Line states */
typedef enum { NewLine, NewLineNC, PosComment, CppComment,
               Comment, PosEndComment, EndComment, CompDir,
               LineOfCode, OpenBracket, CloseBracket1,
               CloseBracket2, PosEOL, InlineComment } STATETYPE;

/* functions to determine line states */
STATETYPE next_new_line(char ch);
STATETYPE next_pos_comment(char ch);
STATETYPE next_cpp_comment(char ch);
STATETYPE next_comment(char ch);
STATETYPE next_pos_end_comment(char ch);
STATETYPE next_comp_dir(char ch);
STATETYPE next_line_of_code(char ch);
STATETYPE next_open_bracket(char ch);
STATETYPE next_close_bracket1(char ch);
STATETYPE next_close_bracket2(char ch);
STATETYPE next_pos_eol(char ch);
STATETYPE next_inline_comment(char ch);

/* display functions */
void printLoc(char source[], int loc);
void show_usage(char p_name[]);
