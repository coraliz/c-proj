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
#include <math.h>

#include "structs.h"
#include "defines.h"

/*boolean enum*/
/*enum boolean {false=0, true=1};*/
typedef enum booleans {false=0, true=1} bool;

/*Directive enum - used during the first pass to determine which directive has been recieved.*/
typedef enum directiveTypes {NOT_A_DIRECTIVE, DATA_DIRECTIVE, STRING_DIRECTIVE, ENTRY_DIRECTIVE, EXTERN_DIRECTIVE, UNDEFINED_DIRECTIVE} directiveType;
/*the values also represents the addresing method, ecept to label which is not a method*/
enum operandTypes {NONE=-1, IMMEDIATE=0, DIRECT=1, RELATIVE=2, REGISTER=3, LABEL=4};
/*A->absolute, RELOCATABLE->relative, EXTERNAL->external/entry*/
typedef enum eraTypes {ABSOLUTE='A', RELOCATABLE='R', EXTERNAL='E'} eraTypes;



/*Directive string constants, to be compared with input*/

#define DATA_DIRECTIVE_STR ".data"
#define STRING_DIRECTIVE_STR ".string"
#define ENTRY_DIRECTIVE_STR ".entry"
#define EXTERN_DIRECTIVE_STR ".extern"

/*Maximum line buffer size*/
#define MAX_FILENAME 50
#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 30 /* the length including the ':' is 31*/

#define EXTERNAL_SYMBOL_ADDRESS 0


/*Number of registers*/
#define NUM_OF_REGISTERS 8
#define INTITIAL_IC_VALUE 100 
#define INTITIAL_DC_VALUE 0 

/*Maximum and minimum values that can be stored in 10 bits*/
#define MAX_NUMBER_DATA 2047 /*todo: how??*/
#define MIN_NUMBER_DATA -2048
#define MAX_NUM_IMMEDIATE 511 /*fixed*/
#define MIN_NUMBER_IMMEDIATE -511
/*Amoount of opcodes*/
#define OPCODES_COUNT 16

/*Extrnsions for file i/o*/
#define AS_EXTENSION ".as"
#define TXT_EXTENSION ".txt"
#define OB_EXTENSION ".ob"
#define ENT_EXTENSION ".ent"
#define EXT_EXTENSION ".ext"

/*Opcodes*/
#define OPCODE_MOV "mov"
#define OPCODE_CMP "cmp"
#define OPCODE_ADD "add"
#define OPCODE_SUB "sub"
#define OPCODE_NOT "not"
#define OPCODE_CLR "clr"
#define OPCODE_LEA "lea"
#define OPCODE_INC "inc"
#define OPCODE_DEC "dec"
#define OPCODE_JMP "jmp"
#define OPCODE_BNE "bne"
#define OPCODE_RED "red"
#define OPCODE_PRN "prn"
#define OPCODE_JSR "jsr"
#define OPCODE_RTS "rts"
#define OPCODE_STOP "stop"


/*my mass*/
#define NUM_OF_IMMEDIATE_OPERANDS 5
#define NUM_OF_RELAVITVE_OPERANDS 3
#define IDENTICAL 0;


/*errorHandler.c functions*/
void addError(char * err, int line, char * str);
void printErrors(char * filename);
void freeErrors();
int hasError();
int getMaxLine();


#endif
