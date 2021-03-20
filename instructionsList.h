#ifndef instructionsList_H
#define instructionsList_H

#include "header.h"

/*definition of first word type*/

/*This structure is intended to represent the first instruction words whose represents the instruction general details*/
typedef struct commandWords
{
  char name[6];
  char era;
  int decimalAddress;
  unsigned short int machineCode;
} commandWord;

/*This structure is intended to represent the instruction words of the operands that represent the source and destination*/
typedef struct operandWords
{
  char era;
  int decimalAddress;
  char *labelName;
  int isExtern;
  int isRelative;
  int hasMachineCode;
  int fileLineNumber;
  unsigned short int machineCode;
} operandWord;

/*This structure is intended to represent all the words which relevant to any instruction written in the output file*/
typedef struct instructionNodes
{
  commandWord commandWord;
  operandWord sourceOperandWord;
  operandWord destinationOperandWord;
  struct instructionNodes *next;
} instructionNode;

/*functions*/
/*This function gets new instructionNode and adds it to the linked instructions list*/
void addInstructionNode(instructionNode *node);

/*This function receives an instructionNode pointer and frees up the allocated memory for it*/
void freeInstruction(instructionNode *instructionNodePtr);

/*This function frees up all of the instructions list allocated memory*/
void freeInstructions();

/*This function each time finds the first word that is a label and its address has not yet been defined and return it's pointer*/
operandWord *getUndefinedLabelWord();

/*The function returns a pointer to the beginning of the instructions linked list*/
instructionNode *getInsructionsHead();

#endif
