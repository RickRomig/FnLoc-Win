/*
 * FILE
 *      fnloc.c - version 2.2.1
 * NAME
 *      Copyright 2018 Richard B. Romig
 * EMAIL
 *      rick.romig@gmail.com
 * DATE
 *      6 September 2018
 * DESCRIPTION
 * Counts logical lines of code, skipping comments and blank lines.
 * Count number of functions and lines of code within the functions.
 * Prints a summary upon completion showing the number of functions, total
 * lines of code contained in functions, total lines of code outside of
 * functions, and total lines of code in the program. If the source code
 * contains no functions, as in a header file, it displays that it contains
 * no functions and shows the tocal LOC for the file.
 *
 * LIMITATIONS
 *  Target file must be a text file of C/C++ source code. Results for C source
 *  code written in old style style may not be correct as they may not conform
 *  to ANSI/ISO coding standards.
 ******************************************************************************
 *
 *  To be counted properly functions must take the following form:
 *  int function_name(int x)
 *  {
 * 	statements:
 *  }
 *
 * Function headers that are split into two lines, are counted as functions,
 * their function loc is counted correctly and both lines of the  header are
 * displayed in the output.
 ******************************************************************************
 *
 * Data structures must be written in the following form:
 *  struct foo {
 *   int a;
 *   float b;
 *  };
 *
 * Structures written with the opening braces in the first column will be
 * processed as if they are functions.
 *
 * In data structure declarations such as arrays or enumerated types in which
 * the data elements are delimited by commas, the elements inside the braces
 * are not counted if they are not on the same line as a brace. The first and
 * last lines (the lines with the braces) of the declaration are counted.
 ******************************************************************************
 *
 * if, else, for, while, do constructs without braces and only one statement
 *  ending with ';' following will be counted as one line of code.
 *
 * if ( condition )
 * 	action;
 *
 * for ( i = 1; i < 10; i++ )
 * 	if ( condition )
 * 		action;
 *
 * These will each be counted as one line of code when the first ';' at the
 * end of a line is reached.
 ******************************************************************************
 *
 * COMPILER INFO
 *      gcc (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609 on Linux Mint 18.3
 *      MinGW gcc on Windows 7
 *
 * MODIFICATION HISTORY
 * 4 Sep 2018 	Fixed the problem with counting and identifying mult-line
 * 		function headers.
 * 6 Sep 2018 	Implemented a singly linked list to replace the stack so that
 *		the function listing in the output would be in the same order
 *		as the source code file.
 *		Simplified the data structures. Data is input to the linked
 *		list from temporary variables in main()
 *		Replaced the function to print the output into three separate
 *		functions to display information about the program itself,
 *		another to display function names and loc (if any), and a
 *		function to display a summary of the results.
 * 19 Nov 2018  Modified data structures to hold a possible second line of a
 *		function header.
 *		Revised code to initialize string variables to hold function
 *		names, copy line buffers to them and reset to empty.
 *		Modified linked list functions to handle additional data.
 ******************************************************************************
 *
 * GNU Public License, Version 2
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fnloc.h"

int main(int argc, char *argv[])
{
	/* buffers */
	char buffer[BUF_LEN];	/* line of source code being examined */
	char fn_name1[BUF_LEN];	/* function name */
	char fn_name2[BUF_LEN];	/* 2nd line of function name */

	int i;			/* loops index */
	int prg_loc = 0;	/* running loc count */
	int fn_loc = 0;		/* lines of code in current function */
	int fn_count = 0;	/* running function count */
	int total_fn_loc = 0;	/* running function loc count */

	/* initial line and function states */
	STATETYPE state = NewLine;
	FNSTATETYPE fn_state = NotFunction;
	strcpy(fn_name1, "");
	strcpy(fn_name2, "");

	if ( argc < 2 )
	{
		fprintf(stderr, "No source code file passed.\n");
		show_usage(argv[0]);
		exit(1);
	}

	if ( (strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0) )
	{
		 show_usage(argv[0]);
		 exit(1);
	}

	fp = fopen(argv[1], "r");
	if ( fp == NULL )
	{
		fprintf(stderr, "Cannot open %s\n", argv[1]);
		show_usage(argv[0]);
		exit(1);
	}

	while ( !feof(fp) )
	{
		if ( fgets(buffer, BUF_LEN, fp) )
		{
			for ( i= 0; i < strlen(buffer); i++ )
			{
				switch (state)
				{
				case (NewLine) :
					state = next_new_line(buffer[i]);
					break;
				case (NewLineNC) :
					break;
				case (PosComment) :
					state = next_pos_comment(buffer[i]);
					break;
				case (CppComment) :
					state = next_cpp_comment(buffer[i]);
					breaFILE *fp;k;
				case (Comment) :
					state = next_comment(buffer[i]);
					break;
				case (PosEndComment) :
					state = next_pos_end_comment(buffer[i]);
					break;
				case (EndComment) :
					state = NewLineNC;
					break;
				case (CompDir) :
					state = next_comp_dir(buffer[i]);
					break;
				case (LineOfCode) :
					state = next_line_of_code(buffer[i]);
					break;
				case (OpenBracket) :
					state = next_open_bracket(buffer[i]);
					break;
				case (CloseBracket1) :
					state = next_close_bracket1(buffer[i]);
					break;
				case (CloseBracket2) :
					state = next_close_bracket2(buffer[i]);
					break;
				case (PosEOL) :
					state = next_pos_eol(buffer[i]);
					break;
				case (InlineComment) :
					state = next_inline_comment(buffer[i]);
					break;
				}	/* end switch(state) */
			}	/* end for loop */

			if ( isalpha(buffer[0]) )
			{
				fn_state = PosFunction;
				strcpy(fn_name1, buffer);
				strcpy(fn_name2, "");
				fn_loc = 0;
			}

			if ( fn_state == PosFunction )
			{
				switch (buffer[0])
				{
					case '{':
						fn_state = IsFunction;
						fn_count++;
						break;
					case ' ':
					case '\t':
						strcpy(fn_name2, buffer);
						break;
					case '}':
						fn_state = NotFunction;
						strcpy(fn_name1, "");
						strcpy(fn_name2, "");
				}
			}

			if ( state == NewLine )
				prg_loc++;

			if ( state == NewLine && fn_state == IsFunction )
			{
				fn_loc++;
				total_fn_loc++;
			}

			if ( state == NewLineNC )
				state = NewLine;

			if ( fn_state == IsFunction && buffer[0] == '}' )
			{
				insert_at_end(fn_name1, fn_name2, fn_loc);
				fn_state = NotFunction;
				strcpy(fn_name1, "");
				strcpy(fn_name2, "");
				fn_loc = 0;
			}
		}	/* end if( fgets(... */
	}	/* end while (!feof(fp)) loop */

	/* Display output */
	print_intro(argv[1]);
	print_fn_data(argv[1], fn_count, prg_loc);
	if ( fn_count != 0 )
		print_summary(fn_count, total_fn_loc, prg_loc);

	/* Clean up */
	fclose(fp);
	head = free_list(head);

	return 0;
}

/*
 * FUNCTION
 *	STATETYPE next_new_line(char ch)
 * DESCRIPTION
 *	Determines the next state if in NewLine state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_new_line(char ch)
{
	STATETYPE st;
	if ( ch == '\n' )
		st = NewLineNC;
	else if ( isspace(ch) )
		st = NewLine;
	else
		switch(ch)
		{
			case '/':
				st = PosComment;
				break;
			case '#':
				st = CompDir;
				break;
			case '{':
				st = OpenBracket;
				break;
			case '}':
				st = CloseBracket1;
				break;
			default:
				st = LineOfCode;
				break;
	   	}

	return st;
}

/*
 * FUNCTION
 *	STATETYPE next_pos_comment(char ch)
 * DESCRIPTION
 *	Determines the next state if in PosComment state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_pos_comment(char ch)
{
	STATETYPE st;

	switch (ch)
	{
		case '/':
			st = CppComment;
			break;
		case '*':
			st = Comment;
			break;
		default:
			st = NewLineNC;
			break;
	}

	return st;
}

/*
 * FUNCTION
 *	STATETYPE next_cpp_comment(char ch)
 * DESCRIPTION
 *	Determines the next stat if in CppComment state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_cpp_comment(char ch)
{
    	STATETYPE st;

    	if ( ch == '\n' )
        	st = NewLineNC;
    	else
    		st = CppComment;

    	return st;
}

/*
 * FUNCTION
 *	STATETYPE next_comment(char ch)
 * DESCRIPTION
 *	Determines the next state if in Comment state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_comment(char ch)
{
	STATETYPE st;

	if ( ch == '*' )
		st = PosEndComment;
	else
		st = Comment;

	return st;
}

/*
 * FUNCTION
 *	STATETYPE next_pos_end_comment(char ch)
 * DESCRIPTION
 *	Determines the next state if in PosEndComment state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_pos_end_comment(char ch)
{
	STATETYPE st;

	switch (ch)
	{
		case '/':
			st = EndComment;
			break;
		case '*':
			st = PosEndComment;
			break;
		default:
			st = Comment;
			break;
	}

	return st;
}

/*
 * FUNCTION
 *	STATETYPE next_comp_dir(char ch)
 * DESCRIPTION
 *	Determines the next state if in CompDir state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_comp_dir(char ch)
{
	STATETYPE st;

	if (ch == '\n')
		st = NewLine;
	else
		st = CompDir;

	return st;
}

/*
 * FUNCTION
 *	STATETYPE next_line_of_code(char ch)
 * DESCRIPTION
 *	Determines the next state if in LineOfCode state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 * MODIFICATION HISTORY
 *	09-03-18 Removed case ',' and case ':'
 */
STATETYPE next_line_of_code(char ch)
{
	STATETYPE st;

	switch (ch)
	{
		case '}':
			st = CloseBracket2;
			break;
		case '{':
		case ';':
			st = PosEOL;
			break;
		default:
			st = LineOfCode;
			break;
	}

	return st;
}

/*
 * FUNCTION
 *	STATETYPE next_open_bracket(char ch)
 * DESCRIPTION
 *	Determines the next state if in OpenBracket state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_open_bracket(char ch)
{
	STATETYPE st;

	switch (ch)
	{
		case '\n':
			st = NewLine;
			break;
		case '}':
			st = CloseBracket2;
			break;
		default:
			st = LineOfCode;
			break;
	}

	return st;
}

/*
 * FUNCTION
 *	STATETYPE next_close_bracket1(char ch)
 * DESCRIPTION
 *	Determines the next state if in CloseBracket1 state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_close_bracket1(char ch)
{
	STATETYPE st;

	if ( ch == '\n' )
		st = NewLineNC;
	else
		st = CloseBracket2;

	return st;
}

/*
 * FUNCTION
 *	STATETYPE next_close_bracket2(char ch)
 * DESCRIPTION
 *	Determines the next state if in CloseBracket2 state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_close_bracket2(char ch)
{
	STATETYPE st;

	if ( ch == ';' )
		st = PosEOL;
	else
		st = LineOfCode;

	return st;
}


/*
 * FUNCTION
 *	STATETYPE next_pos_eol(char ch)
 * DESCRIPTION
 *	Determines the next state if in PosEOL state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_pos_eol(char ch)
{
	STATETYPE st;

	switch (ch)
	{
		case '\n':
			st = NewLine;
			break;
		case ' ':
		case '\t':
			st = PosEOL;
			break;
		case '/':
			st = InlineComment;
			break;
		default:
			st = LineOfCode;
			break;
	}

	return st;
}

/*
 * FUNCTION
 *	STATETYPE next_inline_comment(char ch)
 * DESCRIPTION
 *	Determines the next state if in InlineComment state.
 * PARAMETERS
 *	char ch - a character from the buffer containing the line of
 * 		  code being examined.
 * RETURN VALUE
 *	Returns the new state.
 * SIDE EFFECTS
 *	None known
 * LIMITATIONS
 *	None known
 */
STATETYPE next_inline_comment(char ch)
{
	STATETYPE st;

	if ( ch == '\n' )
		st = NewLine;
	else
		st = InlineComment;

	return st;
}

/*
 * FUNCTION
 *	void insert_at_end(fn_name, char fn_name2[], fn_loc)
 * DESCRIPTION
 *	inserts data into a singly linked list at the head if it is the first
 *	item, otherwise at the end.
 * PARAMETERS
 *	char fn_name[]	- character string holding the current function name
 *	char fn_name2[] - character strng holding second line of function name
 *	int fn_loc - integer holding the number of loc in the function
 * RETURN VALUE
 *	None, inserts data into the linked list
 */
void insert_at_end(char fn_name1[], char fn_name2[], int fn_loc)
{
	node *current;
	current = (node*)malloc(sizeof(node));

	if ( current == NULL )
	{
		fprintf(stderr, "Out of space\n");
		exit(1);
	}
	else
	{
		strcpy(current->name1, fn_name1);
		strcpy(current->name2, fn_name2);
		current->loc = fn_loc;
		current->next = NULL;

		if ( head == NULL )
		{
			head = current;
			last = current;
		}
		else
		{
			last->next = current;
			last = current;
		}
	}
}

/*
 * FUNCTION
 *	void delete_list(node *head)
 * DESCRIPTION
 *	frees the memory allocated for the list
 * PARAMETERS
 *	node *head - the head of the linked list
 * RETURN VALUE
 *	None
 */
node *free_list(node *head)
{
	node *tmpPtr = head;
	node *followPtr;

	while ( tmpPtr != NULL )
	{
		followPtr = tmpPtr;
		tmpPtr = tmpPtr->next;
		free(followPtr);
	}
	return NULL;
}

/*
 * FUNCTION
 *	void print_intro(char source[])
 * DESCRIPTION
 *	displays introduction for program output
 * PARAMETERS
 *	char source[] - character string containing the name of the source
 *			code file - argv[1]
 * RETURN VALUE
 *	None
 */
void print_intro(char source[])
{
	printf("\nFnLoC 2.2.1\n");
	printf("Copyright 2018, Richard B. Romig\n");
	printf("Licensed under the GNU General Public License, version 2\n\n");
	printf("Lines of code data for %s\n\n", source);
}

/*
 * FUNCTION
 *	void print_fn_data(char source[], int fn_count, int prg_loc)
 * DESCRIPTION
 *	displays function named loc contained in the function.
 *	if there are no functions displays a message to that effect and
 *	displays total lines of coded found in the source file.
 * PARAMETERS
 *	char source[] - character string containing the name of the source
 *			code file - argv[1]
 *	int fn_count - number of functions found
 *	int prg_loc - total lines of code counted in the source file
 * RETURN VALUE
 *	None
 */
void print_fn_data(char source[], int fn_count, int prg_loc)
{
	node *current;
	current = head;

	if ( fn_count == 0 )
	{
		printf("%s does not contain function code.\n\n", source);
		printf("Total Program LOC:   %4d\n\n", prg_loc);
	}
	else
	{
		printf("Functions:\n");
		while ( current != NULL )
		{
			printf("%s", current->name1);
			if ( strcmp(current->name2, "") != 0 )
				printf("%s", current->name2);
			printf("LOC:\t%4d\n", current->loc);
			current = current->next;
		}
	}
}

/*
 * FUNCTION
 *	void print_summary(int fn_count, int total_fn_loc, int prg_loc)
 * DESCRIPTION
 *	displays a summary of loc data.
  * PARAMETERS
 *	int fn_count - number of functions found
 *	int fn_loc - total lines of code counted for all functions
 *	int prg_loc - total lines of code counted in the source file
 * RETURN VALUE
 *	None
 */
void print_summary(int fn_count, int total_fn_loc, int prg_loc)
{
	printf("\nSummary:\n");
	printf("Number of functions: %4d\n", fn_count);
	printf("Function LOC:        %4d\n", total_fn_loc);
	printf("Non-function LOC:    %4d\n", prg_loc - total_fn_loc);
	printf("Total Program LOC:   %4d\n\n", prg_loc);
}

/* FUNCTION
 *	void show_usage(char p_name[])
 * DESCRIPTION
 * 	Displays program usage information if no arguments are passed, file
 * 	cannot be opened or -h or --help is passed.
 * PARAMETERS
 *	char p_name[] - the name of this program (argv[0])
 * RETURN VALUE
 * 	none
 * SIDE EFFECTS
 *	none
 * LIMITATIONS
 * 	none
*/
void show_usage(char p_name[])
{
 	printf("\tUsage: %s filename\n", p_name);
 	printf("\tWhere filename is a C or C++ source code or header file.\n");
 	printf("\tSee README for information regarding style requirements\n");
 	printf("\tand limitations.\n\n");
}
