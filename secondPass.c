#include "secondPass.h"
/*This file contains the second pass function*/

bool secondPass(int IC, int DC)
{
  operandWord *searchedLabelWord = NULL;
  symbol *tmpSymbol = NULL;
  entry *entryLabel = NULL;
  bool errorFlag;

  printf("\n\t###start second pass\n");
  /*If the overall count of words and data exceeds the memory limit, add error and quit*/
  if (((IC-100) + DC) > 4096 )
  {
    addError("Exceeded memory limit", -1, NULL);
    printf("\tERROR: This file exceeded the memory limit.\n"); /*TODO: try to add the name file*/
    return false; /*stop second pass runing*/
  }
  /*LABELS ADDRESSES COMPLETE*/
  /*scan all the existing instructions in the code and search for the words that are labels 
  and therefore missing a machine code (after the first scan)*/
  while ((searchedLabelWord = getLabelWord()))
  {
    printf("\t###works on the label: %s\n", searchedLabelWord->labelName);
    /*search for labels in the list of symbols*/
    tmpSymbol = searchSymbol(searchedLabelWord->labelName);
    /*If no such symbol was found, add an error*/
    if(!tmpSymbol)
    {
      /*in this case we did not find the labels in the symbol table, 
      meaning it was not defined in our assembly code*/
      /*TODO: TAKE IT OFF*/
      char *tmpString;
      tmpString = malloc(sizeof(char)*MAX_LINE_LENGTH);
      strcpy(tmpString, "Label \"");
      strcat(tmpString, searchedLabelWord->labelName);
      strcat(tmpString, "\" was not declared");
      addError(tmpString, searchedLabelWord->fileLineNumber, NULL);
      /*END OF OFF*/

      errorFlag=true;
      printf("\tERROR: The label \'%s\' is not defined (line %d).\n", searchedLabelWord->labelName, searchedLabelWord->fileLineNumber);
    }
    /**SYMBOL WAS FOUND**/
    /*we found the label in the table of symbols and now we will define the appropriate values for it*/
    else if(tmpSymbol->isExternal)
    {
      /*EXTERN LABEL*/
      printf("\t###got to external label case\n");
      searchedLabelWord->era = EXTERNAL;
      searchedLabelWord->mc = ((unsigned short int)EXTERNAL_SYMBOL_ADDRESS);
      searchedLabelWord->isExternal = true;
    }
    else if(searchedLabelWord->isRelative)
    {
      /*RELATIVE LABEL*/
      printf("\t###got to relative label case\n");
      if(!(tmpSymbol->isCode)){
        addError("Only symbol from type 'code' can be assigned as a relative address", searchedLabelWord->fileLineNumber, NULL);
        errorFlag=true;
        printf("\tERROR: The label \'%s\' is not an instruction (line %d).\n", searchedLabelWord->labelName, searchedLabelWord->fileLineNumber);
        /*TODO: make sure the indication is good*/
      }
      else{
        /*use 0xFFF to reset first 4 bit of the address*/
        searchedLabelWord->mc = ((((tmpSymbol->address)-(searchedLabelWord->decimalAddress)))&(0xFFF)); 
      }
    }
    else{
      /*this is a code label*/
      printf("\t###got to code label case\n");
      searchedLabelWord->mc = ((tmpSymbol->address)&0xFFF);
    }
    searchedLabelWord->hasMachineCode = true;
    /*continue to the next word in list that has no address*/
  } /*end of while (labels that are missing address)*/

  /*ENTRY*/
  /* look for all the labels defined as entry and make sure they are defined in the code */
  while ((entryLabel = getUndefinedEntry()))
  {
    printf("\n\t###works on the entry: %s\n", entryLabel->label);
    /*Search for symbols with the same name*/
    tmpSymbol = searchSymbol(entryLabel->label);
    printf("\t###continue");
    printf("\t###continue");
    printf("\t###continue");
    if(!tmpSymbol)
    {
      /*in this case we did not find the entry in the symbol table, 
      meaning it was not defined in our assembly code*/

      /*TODO: TAKE IT OFF*/
      char *tmpString;
      tmpString = malloc(sizeof(char)*MAX_LINE_LENGTH);
      strcpy(tmpString, "Label \"");
      strcat(tmpString, entryLabel->label);
      strcat(tmpString, "\" was not defined");
      addError(tmpString, entryLabel->line, NULL);
      /*END OF OFF*/
      errorFlag=true;
      printf("\tERROR: The entry \'%s\' is not defined (line %d).\n", entryLabel->label, entryLabel->line);
    }
    /*the enry was found in the symbols list*/
    else
    {
      printf("\t###the entry %s was found in symbols list", entryLabel->label);
      if(tmpSymbol->isExternal){
        /*if this condition is true this label has also been declared as extern*/
        /*TODO: OFF*/
        char *tmpString;
        tmpString = malloc(sizeof(char)*MAX_LINE_LENGTH);
        strcpy(tmpString, "Label \"");
        strcat(tmpString, entryLabel->label);
        strcat(tmpString, "\" be declared as entry and extern");
        addError(tmpString, entryLabel->line, NULL);
        /*END OFF*/
        errorFlag=true;
        printf("\tERROR: The entry \'%s\' cannot be defined as entry and extren at the same  (line %d).\n", entryLabel->label, entryLabel->line);
      }
      else{
        /*declare as entry only*/
        printf("\t###change the entry label: %s to true\n", entryLabel->label);
        tmpSymbol->isEntry=true;
      }
    }
    /*mark this entry label as checked in the list*/
    entryLabel->checked = true;
  }/*end of while (entries check)*/

  printf("\n\t###finished second pass\n");
  return (!errorFlag);
}