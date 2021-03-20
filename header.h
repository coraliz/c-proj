/*
Maman 14 by Amit Fisher
How it works:
  Read a line from input.
    -get first token.
    -Determine if first token is a label. if it is, store it in the symbols list.
    -get next token.
    -Is it a directive?
      -If it's .string or .data, store it in the data list and increment DC.
      -If it's .entry, store a new entry with the next token as label.
      -If it's .extern, store it in the symbols list, with next token as label.
    -If it's not a directive, check for command.
    -If it's a command, analize the command and count # of words. increment IC,
      and add the command to the words list.
  Go back to read another line. If there are no more lines:

    Now we have all the symbols and data. Now we'll update every data\string symbols's address.
    If there were errors, we will create an error file and write the errors to that file.
    if there aren't any errors, we will create an object file, and extern/entry files if needed.
*/


/*This file contains enumerations and defines of constants*/

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "generalUtils.h" /*TODO: ask Tomer*/

/*boolean enum*/
typedef enum booleans {false=0, true=1} bool;

/*defines*/
#define INITIAL_IC_VALUE 100 
#define INITIAL_DC_VALUE 0 
#define DECIMAL_BASE 10

/*Maximum and minimum values that can be stored in 12 bits*/
#define MAX_NUMBER_VALUE 2047
#define MIN_NUMBER_VALUE -2048

#define MAX_LINE_LENGTH 81 /*maximum line buffer size- includes the char '\n' */
#define MAX_LABEL_LENGTH 31 /*count the '/0' char*/
#define EXTERNAL_SYMBOL_ADDRESS 0

/*era types as enum - A->absolute, RELOCATABLE->relative, EXTERNAL->external/entry*/
typedef enum eraTypes {ABSOLUTE='A', RELOCATABLE='R', EXTERNAL='E'} eraType;


#endif
