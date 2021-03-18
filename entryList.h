#ifndef entryList_H
#define entryList_H

#include "header.h"
#include "utilsFirstPass.h"

/*structures*/
typedef struct entryTable
{
  char *label;
  int checked;
  int line;
  struct entryTable *next;
} entry;


/*functions*/
void addEntry(char * label, int line);
void freeEntries();
int doesEntryExist(char * label);
entry * getUndefinedEntry();
entry * getEntryHead();

#endif
