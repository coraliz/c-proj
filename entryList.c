/*#include "header.h"*/
#include "entryList.h"

static entry *entryHead = NULL;

/*This functions adds an entry to the list. it takes a label and a line, for error reporting*/
void addEntry(char *label, int line)
{
  /*temp pointer to entry*/
  entry *newEntry = NULL;
  /*Check if a similar entry exists*/
  printf("\n\t###start to added the entry: %s\n", label);
  if (doesEntryExist(label))
  {
    return;
  }
  /*allocate memory for new node*/
  newEntry = (entry *)malloc(sizeof(entry));
  checkAllocation(newEntry);
  /*copy the label*/
  newEntry->label = malloc(sizeof(char)*MAX_LABEL_LENGTH);
  strcpy(newEntry->label, label);
  newEntry->line = line;
  newEntry->checked = false;
  newEntry->next = NULL;

  if (!entryHead)
  {
    entryHead = newEntry;
  }
  else
  {
    entry *p = entryHead;
    while (p->next)
      p = p->next;
    p->next = newEntry;
  }
  printf("\t###added the entry: %s\n", newEntry->label);
}


/*This function checks if an entry with the same label already exists
If there is one, it will return 0. else, it will return 1;*/
int doesEntryExist(char * label)
{
  entry *ptr = entryHead;
  /*Go through the list*/
  while (ptr)
  {
    /*If we find a node with the same label*/
    if (!strcmp(ptr->label, label))
    {
      return true;
    }
    ptr = ptr->next;
  }
  /*If such node doesn't exist, return 1*/
  return false;
}

/*This function frees all the nodes in this list*/
void freeEntries()
{
  entry * ptr = entryHead;
  if (!ptr)
    return;
  while (ptr)
  {
    entry * tmp = ptr;
    ptr = ptr->next;
    free(tmp->label);
    free(tmp);
  }
  entryHead = NULL;
  printf("\n\t###entries are free\n");
}


/*This function loops through the entries, and returns
an entry without an address assigned to it. for use during the
second pass*/
entry * getUndefinedEntry()
{
  entry *ptr = entryHead;

  printf("\n\t###in getUndefinedEntry\n");
  while (ptr)
  {
    printf("\t###generic is: %s\n", ptr->label);
    if (ptr->checked==false)
    {
      printf("\t###undefinedEntry is: %s\n", ptr->label);
      return ptr;
    }
    ptr = ptr->next;
  }
  printf("\t###out getUndefinedEntry\n");
  return NULL;
}

/*This function returns a pointer to the head*/
entry * getEntryHead()
{
  entry *tmp = entryHead;
  return tmp;
}
