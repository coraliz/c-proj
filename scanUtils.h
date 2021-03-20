#ifndef scanUtils_H
#define scanUtils_H

#include "header.h"
#include "symbolsList.h"
#include "instructionsList.h"

/*enums*/
/*directive types as enum - used during the first pass to determine which directive has been recieved.*/
typedef enum directiveTypes {NOT_A_DIRECTIVE, DATA_DIRECTIVE, STRING_DIRECTIVE, ENTRY_DIRECTIVE, EXTERN_DIRECTIVE, UNDEFINED_DIRECTIVE} directiveType;

/*operand types as enum - the values also represents the addresing method, except to label which is not a method*/
typedef enum operandTypes {NONE=-1, IMMEDIATE=0, DIRECT=1, RELATIVE=2, REGISTER=3, LABEL=4} operandType;

/*Directive string constants, to be compared with input*/
#define DATA_DIRECTIVE_STR ".data"
#define STRING_DIRECTIVE_STR ".string"
#define ENTRY_DIRECTIVE_STR ".entry"
#define EXTERN_DIRECTIVE_STR ".extern"

/*structures*/
/*This structure represents the various instruction that exist in an assembly language - 
name, opcode, funct and operands count (how many operands the command should receive)*/
typedef struct instructions
{
  char *name;
  unsigned int opcode : 4;
  unsigned int funct : 4;
  unsigned int operandCount : 2;
} instruction;

/*functions*/
/*This function receives a token from the command line and returns whether it is a register or not*/
bool isRegister(char *token);

/*This function receives a token from the command line and if it is a defined instruction, returns a pointer to it. 
Otherwise returns NULL*/
instruction *getInstruction(char *token);

/*This function gets a token from the command line and checks if it is a legal label. 
If so, returning true. Otherwise prints an error and returns false.*/
bool isLegalLabel(char *token, int lineNumber);

/*This function gets the parameters line in the command and its line number in the input file. 
Its purpose is to check if the command is written in the correct convention in terms of the number of commas that appear in it 
and that does not start or end with a comma.*/
bool isLegalCommadConvention(char *paramsLine, int fileLineNumber);

#endif
