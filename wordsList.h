#ifndef wordList_H
#define wordList_H

#include "header.h"


/*definition of first word type*/
typedef struct commandWords
{
  char name[6];
  char era;
  int decimalAddress;
  unsigned short int mc;
} commandWord;


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
/*end of structures*/

/*functions*/
void addWordNode(wordNode *node);
void freeWord(wordNode *wordPtr);
void freeWords();
operandWord *getLabelWord();
wordNode *getWordsHead();
/*end of functions*/


#endif
