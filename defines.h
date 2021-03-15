#ifndef DEFINES_H
#define DEFINES_H

/*This file contains enumerations and defines of constants*/

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
#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LENGTH 30 /* the length including the ':' is 31*/

#define EXTERNAL_SYMBOL_ADDRESS 0

/*Maximum filename size*/
#define MAX_FILENAME 50

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
# define IDENTICAL 0;


#endif
