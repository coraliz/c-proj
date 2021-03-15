#ifndef HEADER_H
#define HEADER_H
/* Includes */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "structs.h"
#include "defines.h"

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

/*files.c functions*/
FILE * openFile(char* filename, char * mode, char * extension);
void exportFiles(int IC, int DC, char * name);

/*firstPass.c functions*/
void firstPass(FILE * f, int * counter, int * DC);

/*secondPass.c functions*/
void secondPass(int IC, int DC);

/*errorHandler.c functions*/
void addError(char * err, int line, char * str);
void printErrors(char * filename);
void freeErrors();
int hasError();
int getMaxLine();

/*symbolList.c functions*/
void addSymbol(char *label, int address, int isCode, int isData, int isExternal, int isEntry, int lineNumber);
void freeSymbols();
void doesSymbolExist(char * label, int line);
symbol * searchSymbol(char * label);
void updateDataSymbols(int IC);



/*utilsFirstPass.c*/
int isLegalLabel(char *token, int lineNumber);
int isLablelConvention(char *token);
void checkAllocation(void * ptr);
directiveType getDirectiveType(char * token);
/*int getDirectiveType(char * token);*/
bool isComment(char * token);

/*entryList.c functions*/
void addEntry(char * label, int line);
void freeEntries();
int doesEntryExist(char * label);
entry * getUndefinedEntry();
entry * getEntryHead();

/*operations.c functions*/

/*wordsList.c functions*/
void addWordNode(wordNode *node);
void freeWords();
operandWord *getLabelWord();
wordNode *getWordsHead();
int countWords();
/*void updateAddresses();*/


/*my mass!!!!*/
bool isImmediateAddress(char *token);
bool isRelative(char * token);
bool isRegister(char * token);
bool isOperation(char *token);
bool isLegalImmediateAddress(char *token, int fileLineNumber, int *num);
void setCommandWord(wordNode *wordNodePtr, instruction *currentInstruction, int *IC);
bool setImmediateOperandWord(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
bool setRegisterOperandWord(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
bool setRelativeOperand(wordNode *wordNodePtr, char *token, int fileLineNumber, int *ICs);
bool setLabelOperand(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
int getTokenType(char * token);
void setOperandWord(bool *errorFlag, wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
void analyzeOperation(char * token, int fileLineNumber, int *IC, char *label);


int isValidNumber(char *numberStr, int fileLineNumber, long int *currentNumber);
void addData(unsigned short int value, int *DC);

bool isDirectiveConvention(char * token);
void setExternal(char *type, int fileLineNumber);
symbol *getSymbolHead();

bool isLegalCommadConvention(char *commandLine, int fileLineNumber);
int iscomma(char c);

#endif
