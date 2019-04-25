/*
 * FILE
 *     lloc.c
 * NAME
 *      Copyright 2018 Richard B. Romig
 * EMAIL
 *      rick.romig@gmail.com
 * DATE
 *      25 January 2019
 * DESCRIPTION:
 * Counts logical lines of code, skipping comments and blank lines, reporting
 * the number of logical lines of code counted.
 * LIMITATIONS
 * Target file must be a text file of C/C++ source code. Results for C source
 * code written in old style style may not be correct as they may not conform
 * to ANSI/ISO coding standards.
 *
 * In data structure declarations such as arrays or enumerated types in which
 * the data elements are delimited by commas, the elements inside the braces
 * are not counted if they are not on the same line as a brace. The first and
 * last lines (the lines with the braces) of the declaration are counted.
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
 *
 * COMPILER INFO
 *      gcc (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609 on Linux Mint 18.3
 *      MinGW gcc on Windows 7
 * MODIFICATION HISTORY
 * 25 Jan 2019  Modified original source code from 1998 Computer Engineering
 * class project to incorporate code from FnLoC 2.2.1 project.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lloc.h"

int main(int argc, char *argv[])
{
        FILE *fp;
        char buffer[BUFF_LEN];
        STATETYPE state = NewLine;
        int loc = 0;
        int i;

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
        if( fp == NULL )
        {
                fprintf(stderr, "Cannot open %s\n", argv[1]);
                show_usage(argv[0]);
                exit(1);
        }

        while( !feof(fp) )
        {
                if( fgets(buffer, BUFF_LEN, fp) )
                {
                        for( i = 0; i < strlen(buffer); i++ )
                        {
                                switch(state)
                                {
                                case(NewLine) :
                                        state = next_new_line(buffer[i]);
                                        break;
                                case (NewLineNC) :
				        break;
                                case(PosComment) :
                                        state = next_pos_comment(buffer[i]);
                                        break;
                                case(CppComment) :
                                        state = next_cpp_comment(buffer[i]);
                                        break;
                                case(Comment) :
                                        state = next_comment(buffer[i]);
                                        break;
                                case(PosEndComment) :
                                        state = next_pos_end_comment(buffer[i]);
                                        break;
                                case(EndComment) :
                                        state =  NewLineNC;
                                        break;
                                case(CompDir) :
                                        state = next_comp_dir(buffer[i]);
                                        break;
                                case(LineOfCode) :
                                        state = next_line_of_code(buffer[i]);
                                        break;
                                case(OpenBracket) :
                                        state = next_open_bracket(buffer[i]);
                                        break;
                                case(CloseBracket1) :
                                        state = next_close_bracket1(buffer[i]);
                                        break;
                                case(CloseBracket2) :
                                        state = next_close_bracket2(buffer[i]);
                                        break;
                                case(PosEOL) :
                                        state = next_pos_eol(buffer[i]);
                                        break;
                                case(InlineComment) :
                                        state = next_inline_comment(buffer[i]);
                                        break;
                                } /* end switch(state) */
                        }  /* end for loop */

                        if( state == NewLine )
                                loc++;
                        else if( state == NewLineNC )
                                state = NewLine;
                } /* end if( fgets(... */
        } /* end while loop */

        printLoc(argv[1], loc);

        /* Clean up - close the file */
        fclose(fp);

        return(0);
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

        if(ch == '\n' )
                st = NewLineNC;
        else if( isspace(ch) )
                st = NewLine;
        else
                switch (ch)
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

        if( ch == '\n' )
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
 *	void print_intro(char source[])
 * DESCRIPTION
 *	displays introduction for program output
 * PARAMETERS
 *	char source[] - character string containing the name of the source
 *			code file - argv[1]
 * RETURN VALUE
 *	None
 */
void printLoc(char source[], int loc)
{
	printf("\nLLoC 1.0\n");
	printf("Copyright 2019 Richard B. Romig\n");
	printf("Licensed under the GNU General Public License, version 2\n\n");
	printf("Lines of code for %s:\t%d\n\n", source, loc);
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
