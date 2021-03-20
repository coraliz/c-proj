#ifndef entryList_H
#define entryList_H

#include "header.h"

/*structures*/
/*Using this structure we save all labels declared as 'entry' in the given input file*/
typedef struct entryNodes
{
  char *label;
  int checked;
  inputFileLine *line;
  struct entryNodes *next;
} entryNode;

/*functions*/

/*This function gets the entry label name and the file line number. 
It adds the new entry node to the linked entries list after defining all its details using the label name 
and the line number appears in the code.*/
void addEntry(char *label, inputFileLine *fileLine);

/*This function frees all the memory structures we used to define the linked list*/
void freeEntries();

/*This function gets a label name.
It checks if the label name is already set in the list of entries. 
If so, then returns true, otherwise returns the false value*/
bool doesEntryExist(char *label);

/*This returns all entryNodes of the entry labels that we have not yet checked if indeed defined as a label in the input file*/
entryNode *getUndefinedEntry();

/*The function returns a pointer to the beginning of the linked list of the entries*/
entryNode *getEntryHead();

#endif
