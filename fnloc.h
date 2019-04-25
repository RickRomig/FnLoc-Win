/*
 * fnloc.h - header file for fnloc.c version 2.2.1
 * Copyright (C) 2018  Richard Romig
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
 * 6 September 2018
 */

/* Maximum buffer length */
#define BUF_LEN 128

FILE *fp;

/* linked list data structures */
struct fn_data {
	char name1[BUF_LEN];
	char name2[BUF_LEN];
	int loc;
	struct fn_data *next;
};

typedef struct fn_data node;

node *head;
node *last;

/* Line states */
typedef enum {
	NewLine, NewLineNC, PosComment, CppComment, Comment,
	PosEndComment, EndComment, CompDir, LineOfCode, OpenBracket,
  	CloseBracket1, CloseBracket2, PosEOL, InlineComment
} STATETYPE;

/* Function states */
typedef enum { NotFunction, PosFunction, IsFunction } FNSTATETYPE;

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

/* functions for the list */
void insert_at_end(char fn_name1[], char fn_name2[], int fn_loc);
node *free_list(node *head);

/* display functions */
void print_intro(char source[]);
void print_fn_data(char source[], int fn_count, int prg_loc);
void print_summary(int fn_count, int total_fn_loc, int prg_loc);
void show_usage(char p_name[]);
