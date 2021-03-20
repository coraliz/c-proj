#include "symbolsList.h"

static symbol *symbolHead = NULL;

/*add new symbol to list*/
void addSymbol(char *label, int address, int isCode, int isData, int isExtern, int isEntry, int lineNumber, bool *errorFlag)
{
  symbol *newSymbol = NULL;
  if(!doesSymbolExist(label, lineNumber))
  {
    /*Allocate memory for the new symbol*/
	  newSymbol = (symbol *)malloc(sizeof(symbol));
    checkMemoryAllocation(newSymbol);
	  /*set new symbol details*/
	  /*allocate memory for the new symbol name (label)*/
	  newSymbol->label = malloc(sizeof(char)*MAX_LABEL_LENGTH);
    checkMemoryAllocation(newSymbol->label);
	  /*save the label name from the input*/
	  strcpy(newSymbol->label, label);
	  newSymbol->address = address;
	  newSymbol->isCode = isCode;
	  newSymbol->isData = isData;
	  newSymbol->isExtern = isExtern;
	  newSymbol->isEntry = isEntry;
	  newSymbol->next = NULL;

    /*add the new symbol to the linked list*/
    if (!symbolHead)
    {
      symbolHead = newSymbol;
    }
    else{
      symbol *ptr = symbolHead;
      while (ptr->next)
      ptr = ptr->next;
      ptr->next = newSymbol;
    }
  }
  else{
    *errorFlag=true;
  }
}

void updateDataSymbols(int IC)
{
  symbol *ptr = symbolHead;
  while (ptr)
  {
    if (ptr->isData == true)
    {
      ptr->address += IC;
    }
    ptr = ptr->next;
  }
}

void freeSymbols()
{
  symbol *ptr = symbolHead;
  if (!ptr){
    return;
  }
  while (ptr)
  {
    symbol *prevPtr = ptr;
    ptr = ptr->next;
    free(prevPtr->label);
    free(prevPtr);
  }
  symbolHead = NULL;
}

bool doesSymbolExist(char *label, int line)
{
  symbol *ptr = symbolHead;
  while (ptr)
  {
    if (!strcmp(ptr->label, label))
    {
      printf("\tERROR: There are multiple symbols with the name: %s (line %d).\n", label, line);
      return true;
    }
    ptr = ptr->next;
  }
  return false;
}

symbol *searchSymbol(char * label)
{
  symbol *ptr = symbolHead;
  while(ptr)
  {
    if (!strcmp(ptr->label, label))
    {
      return ptr;
    }
    ptr = ptr->next;
  }
  return NULL;
}  

symbol *getSymbolHead()
{
  symbol *ptr = symbolHead;
  return ptr;
}

