#include "header.h"
/*This file contains the second pass function*/

void secondPass(int IC, int DC)
{
  operandWord *searchedLabelWord = NULL;
  symbol *tmpSymbol = NULL;
  entry *entryLabel=NULL;

  printf("\t##start second pass\n");
  /*If the overall count of words and data exceeds the memory limit, add error and quit*/
  if (((IC-100) + DC) > 4096 )
  {
    addError("Exceeded memory limit", -1, NULL);
    printf("\t##Exceeded memory limit\n");
    return;
  }
  /*LABELS*/
  /*Loop that gets every label which is missing an address*/
  while ((searchedLabelWord = getLabelWord()))
  {
    printf("\t###works on the label: %s\n", searchedLabelWord->labelName);
    /*search for labels in the list of symbols*/
    tmpSymbol = searchSymbol(searchedLabelWord->labelName);
    /*If no such symbol was fo0und, add an error*/
    if(!tmpSymbol)
    {
      char *tmpString;
      tmpString = malloc(sizeof(char)*MAX_LINE_LENGTH);
      strcpy(tmpString, "Label \"");
      strcat(tmpString, searchedLabelWord->labelName);
      strcat(tmpString, "\" was not declared");
      addError(tmpString, searchedLabelWord->fileLineNumber, NULL);
      printf("\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^Regular label Wasn't declared : %s\n", searchedLabelWord->labelName);
    }
    /*SYMBOL WAS FOUND*/
    /*change the details of the operand word depending on the type of symbol*/
    else if(tmpSymbol->isExternal)
    {
      /*EXTERN OPERAND*/
      printf("\t###got to external label case\n");
      searchedLabelWord->era = EXTERNAL;
      searchedLabelWord->mc = ((unsigned short int)EXTERNAL_SYMBOL_ADDRESS);
      searchedLabelWord->isExternal = true;
    }
    else if(searchedLabelWord->isRelative)
    {
      /*RELATIVE OPERAND*/
      printf("\t###got to relative label case\n");
      /*use 0xFFF to reset first 4 bit of the address*/
      searchedLabelWord->mc = ((((tmpSymbol->address)-(searchedLabelWord->decimalAddress)))&(0xFFF)); 
    }  
    /*otherwise it is an entry label*/
    else{
      /*CODE OPERAND (regular label)*/
      printf("\tgot to entry label case\n");
      searchedLabelWord->mc = ((tmpSymbol->address)&0xFFF);
    }
    /*continue to the next operand that has no address*/
    searchedLabelWord->hasMachineCode = true;
  }
  /*ENTRY*/
  /*check if all objects declared as entry in the code are indeed defined*/
  while ((entryLabel = getUndefinedEntry()))
  {
    printf("\t###works on the entry: %s\n", entryLabel->label);
    /*Search for symbols with the same name*/
    tmpSymbol = searchSymbol(entryLabel->label);
    /*If there is no such symbol, add an error*/
    if(!tmpSymbol)
    {
      /*if the symbol is not in the symbols list, then it is not defined*/
      /*add an error for that*/
      char *tmpString;
      tmpString = malloc(sizeof(char)*MAX_LINE_LENGTH);
      strcpy(tmpString, "Label \"");
      strcat(tmpString, entryLabel->label);
      strcat(tmpString, "\" was not defined");
      addError(tmpString, entryLabel->line, NULL);
      printf("\n\t^^^^^^^^^^^^^^^^^^^^^^^^^^^^Entry label Wasn't declared : %s\n", searchedLabelWord->labelName);
    }
    /*the enry was found in the symbols list*/
    else
    {
      if(tmpSymbol->isExternal){
        char *tmpString;
        tmpString = malloc(sizeof(char)*MAX_LINE_LENGTH);
        strcpy(tmpString, "Label \"");
        strcat(tmpString, entryLabel->label);
        strcat(tmpString, "\" be declared as entry and extern");
        addError(tmpString, entryLabel->line, NULL);
        return;
      }
      else{
        /*declare as entry only*/
        printf("\t###change the entry label: %s to true", entryLabel->label);
        tmpSymbol->isEntry=true;
      }
    }
    /*mark theis entry label as checked in the list*/
    entryLabel->checked = true;
  }
  printf("finished second pass\n");
}