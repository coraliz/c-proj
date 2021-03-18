#include "symbolsList.h"

static symbol *symbolHead = NULL;

/*add new symbol to list*/
void addSymbol(char *label, int address, int isCode, int isData, int isExternal, int isEntry, int lineNumber, bool *errorFlag)
{
  symbol *newSymbol = NULL;
  if(!doesSymbolExist(label, lineNumber))
  {
    /*Allocate memory for the new symbol*/
	  newSymbol = (symbol *)malloc(sizeof(symbol));
	  checkAllocation(newSymbol);
	  /*set new symbol details*/
	  printf("\n\t###add the symbol: *%s* to the symbols list\n", label);
	  /*allocate memory for the new symbol*/
	  newSymbol->label = malloc(sizeof(char)*MAX_LABEL_LENGTH);
	  checkAllocation(newSymbol->label);
	  /*save the label name from the input*/
	  strcpy(newSymbol->label, label);
	  newSymbol->address = address;
	  newSymbol->isCode = isCode;
	  newSymbol->isData = isData;
	  newSymbol->isExternal = isExternal;
	  newSymbol->isEntry = isEntry;
	  newSymbol->next = NULL;
    
    /*add the new symbol to the linked list*/
    if (!symbolHead)
    {
      symbolHead = newSymbol;
    }
    else{
      symbol *ptr = symbolHead;
      /*get to the last node*/
      while (ptr->next)
      ptr = ptr->next;
      /*set last node's next to newSymbol*/
      ptr->next = newSymbol;
    }
  }
  else{
    *errorFlag=true;
  }
}


/*the function updates the value of the addresses of all the symbols defined as data according to the value of IC*/
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
  printf("\t###updated data symbols\n");
}

/*this function releases all the memory cells that hold the symbol details*/
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
  printf("\t###symbols are free\n");
}


/*this function checks if a particular symbol name has already been defined*/
bool doesSymbolExist(char *label, int line)
{
  symbol *ptr = symbolHead;
  while (ptr)
  {
    if (!strcmp(ptr->label, label))
    {
      addError("Multiple symbols with the same name", line, label);
      printf("\tERROR: There are multiple symbols with the name: %s (line %d).\n", label, line);
      return true;
    }
    ptr = ptr->next;
  }
  return false;
}


/*the function looks for a specific name of a symbol in the symbols list, 
if it finds it then it returns the object, otherwise returns NULL*/
symbol *searchSymbol(char * label)
{
  symbol *ptr = symbolHead;
  while(ptr)
  {
    if (!strcmp(ptr->label, label))
    {
      printf("\t###symbol to find this label\n");
      return ptr;
    }
    ptr = ptr->next;
  }
  printf("\t###symbol wasn't found\n");
  return NULL;
}  


/*the function fetch the symbol list head ptr*/
symbol *getSymbolHead()
{
  symbol *tmp = symbolHead;
  return tmp;
}

