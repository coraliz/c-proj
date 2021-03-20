#include "entriesList.h"

static entryNode *entryHead = NULL; /*pointer to the beginning of the entry list*/

bool doesEntryExist(char *label)
{
  entryNode *entryPtr = entryHead;
  while (entryPtr)
  {
    if (!strcmp(entryPtr->label, label))
    {
      /*means we find a node with the same label name*/
      return true;
    }
    entryPtr = entryPtr->next;
  }
  /*we didn't find a node with the same label name*/
  return false;
}

void addEntry(char *label, int fileLineNumber)
{
  entryNode *newEntry = NULL;
  if (doesEntryExist(label))
  {
    /*If this value already exists, no action is required*/
    return;
  }
  /*allocate memory for the new node*/
  newEntry = (entryNode *)malloc(sizeof(entryNode));
  checkMemoryAllocation(newEntry);
  /*copy the label*/
  newEntry->label = malloc(sizeof(char) * MAX_LABEL_LENGTH);
  strcpy(newEntry->label, label);
  /*set entry node values*/
  newEntry->line = fileLineNumber;
  newEntry->checked = false;
  newEntry->next = NULL;
  /*adds entry node to the linked list*/
  if (!entryHead)
  {
    /*first value in the list*/
    entryHead = newEntry;
  }
  else
  {
    /*add the node to the end of the list*/
    entryNode *p = entryHead;
    while (p->next)
      p = p->next;
    p->next = newEntry;
  }
}

void freeEntries()
{
  entryNode *entryPtr = entryHead;
  if (!entryPtr)
    return;
  while (entryPtr)
  {
    entryNode *tmp = entryPtr;
    entryPtr = entryPtr->next;
    free(tmp->label); /*free the label memory allocation*/
    free(tmp);
  }
  entryHead = NULL;
}

entryNode *getUndefinedEntry()
{
  entryNode *entryPtr = entryHead;

  while (entryPtr)
  {
    if (entryPtr->checked == false)
    {
      return entryPtr;
    }
    entryPtr = entryPtr->next;
  }
  return NULL;
}

entryNode *getEntryHead()
{
  entryNode *tmp = entryHead;
  return tmp;
}