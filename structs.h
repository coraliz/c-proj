#ifndef STRUCTS_H
#define STRUCTS_H
/*This file contains */

/*definition of opcode type
used internally in operations.c to loop through all commands*/
typedef struct instructions
{
  char *name;
  unsigned int opcode:4;
  unsigned int funct:4;
  unsigned int operandCount:2;
} instruction;  

struct cMachineCode
{
  unsigned int destination: 2;
  unsigned int source: 2;
  unsigned int funct:4;
  unsigned int opcode:4;
};

/*definition of first word type*/
typedef struct commandWords
{
  char name[6];
  char era;
  int decimalAddress;
  unsigned short int mc;
  struct cMachineCode machineCode;
} commandWord;

typedef union oMachineCodes
{
  unsigned int address: 12;
  int relativeAddress: 12;
} oMachineCode;


typedef struct operandWords
{
  char era;
  int decimalAddress;
  char *labelName;
  int isExternal;
  int isRelative;
  int hasMachineCode;
  int fileLineNumber;
  unsigned short int mc;
  oMachineCode machineCode;
} operandWord;

/*change this name to wordNodes*/
typedef struct wordNodes{
  struct commandWords commandWord;
  operandWord sourceOperandWord;
  operandWord destinationOperandWord;
  struct wordNodes *next;
} wordNode;

/*Word holder union, used to hold all of the words.
a union is used so that every type of word can be in a linked list together*/
typedef union wordHolders
{
  commandWord command;
  unsigned int address: 12;
  int relativeAddress: 12;
} wordHolder;


/*definition of error type. used in the errors list*/
typedef struct errors
{
  char *errorMessage;
  int line;
  char * str;
  struct errors *next;
} error;

typedef struct symbols
{
  char *label;
  int address;
  int isExternal;
  int isEntry;
  int isCode;
  int isData;
  struct symbols *next;
} symbol; 

typedef struct dataNodes
{
  char era;
  int address; /*decimal address*/
  unsigned short int machineCode;
  struct dataNodes *next;
} data;

typedef struct entryTable
{
  char *label;
  int checked;
  int line;
  struct entryTable *next;
} entry;

#endif
