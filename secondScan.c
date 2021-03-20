#include "secondScan.h"

/*This function completes all the words we could not define in the first pass because we did not yet know the addresses relevant 
to each of the labels (the first pass is responsible for building the table of symbols that contains this information). 
If it was able to find all the words and define them returns true, otherwise it takes out the relevant error and returns false.*/
bool setUndefinedLabelWords(){
  operandWord *searchedLabelWord = NULL;
  symbol *tmpSymbol = NULL;
  bool hasError = false;

  /*scan all the existing instructions in the code and search for the words that are labels 
  and therefore missing a machine code (after the first scan)*/
  while ((searchedLabelWord = getUndefinedLabelWord()))
  {
    /*search for the label name in the list of symbols*/
    tmpSymbol = searchSymbol(searchedLabelWord->labelName);
    if(!tmpSymbol)
    {
      /*in this case we did not find the labels in the symbol table, 
      meaning it was not defined in our assembly code*/
      hasError=true;
      printf("\tERROR: The label \'%s\' is not defined (line %d).\n", searchedLabelWord->labelName, searchedLabelWord->fileLineNumber);
    }
    /*we found the label in the table of symbols and now we will define the appropriate values for it*/
    else if(searchedLabelWord->isRelative)
    {
      /*chek first if this label is defined as a relative label*/
      if(tmpSymbol->isCode==false){
        hasError=true;
        printf("\tERROR: The label \'%s\' is not an instruction, threrefore can not be assigned as a relative address (line %d).\n", searchedLabelWord->labelName, searchedLabelWord->fileLineNumber);
        /*TODO: make sure the indication is good*/
      }
      else{
        searchedLabelWord->machineCode = ((unsigned short int)((tmpSymbol->address)-(searchedLabelWord->decimalAddress)));
      }
    }
    else if(tmpSymbol->isExtern)
    {
      /*define extern label word's setting*/
      searchedLabelWord->era = EXTERNAL;
      searchedLabelWord->machineCode = ((unsigned short int)EXTERNAL_SYMBOL_ADDRESS);
      searchedLabelWord->isExtern = true;
    }
    else if(tmpSymbol->isCode){
      /*all that left is that it's a code label*/
      searchedLabelWord->machineCode = ((unsigned short int)tmpSymbol->address);
    }
    searchedLabelWord->hasMachineCode = true; /*continue to the next word in list that has no address*/
  } /*end of while (no more labels that are missing address)*/
  return (!hasError);
}

/*This function is responsible for making sure that all the labels declared as entries are indeed defined in the code. 
If this condition is met returns true, otherwise returns false.*/
bool checkEntriesDefinitions(){
  entryNode *entryLabel = NULL;
  symbol *tmpSymbol = NULL;
  bool hasError = false;

  /*now we will look for all the labels that were declared as entry and make sure they are defined in the code*/
  while ((entryLabel = getUndefinedEntry()))
  {
    /*search for symbols with the same name*/
    tmpSymbol = searchSymbol(entryLabel->label);
    if(!tmpSymbol)
    {
      /*in this case we did not find the entry in the symbol table, 
      meaning it was not defined in our assembly code*/
      hasError=true;
      printf("\tERROR: The entry \'%s\' is not defined (line %d).\n", entryLabel->label, entryLabel->line);
    }
    else
    {
      /*the enry was found in the symbols list*/
      if(tmpSymbol->isExtern){
        /*if this condition is true this label has also been declared as extern which is not allowed*/
        hasError=true;
        printf("\tERROR: The entry \'%s\' cannot be defined as entry and extren at the same code (line %d).\n", entryLabel->label, entryLabel->line);
      }
      else{
        /*was declared as entry only*/
        tmpSymbol->isEntry=true;
      }
    }
    /*mark this entry label as checked in the list*/
    entryLabel->checked = true;
  }/*end of while (checked all entries declarations)*/
  return (!hasError);
}

bool secondScan(int IC, int DC)
{
  /*If the overall count of words and data exceeds the memory limit, add error and return false*/
  if (((IC-INITIAL_IC_VALUE) + DC) > 4096 )
  {
    printf("\tERROR: This file exceeded the memory limit.\n"); /*TODO: try to add the name file*/
    return false;
  }
  if((setUndefinedLabelWords())&(checkEntriesDefinitions())){
    return true;
  }
  return false;
}