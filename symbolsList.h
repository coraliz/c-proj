#ifndef symbolList_H
#define symbolList_H

#include "header.h"
#include "utilsFirstPass.h"

/*structures*/
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

/*functions*/
void addSymbol(char *label, int address, int isCode, int isData, int isExternal, int isEntry, int lineNumber, bool *errorFlag);
void updateDataSymbols(int IC);
void freeSymbols();
bool doesSymbolExist(char *label, int line);
symbol * searchSymbol(char * label);
symbol *getSymbolHead();

#endif
