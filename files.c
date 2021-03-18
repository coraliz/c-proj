#include "files.h"
/*This file contains function used for file handling*/

/*openInputFile function - used to open files.
  takes 3 strings - filename, mode and extension.
  returns a pointer to the opened file, or null if failed*/

FILE * openFile(char* filename, char * mode, char *extension)
{
  /*Temp file to be returned*/
  FILE *tmp = NULL;
  /*Full filename string*/
  char full_filename[MAX_FILENAME_LENGTH];
  /*Copy the filename from argv to the full filename string*/
  strcpy(full_filename, filename);
  /*Add the .as extension to the full filename.*/
  strcat(full_filename, extension);
  /*Open the file. if fopen return null, exit the function and print error to screen*/
  if (!(tmp = fopen(full_filename, mode)))
  {
    printf("Can't open file: %s.\n", full_filename);
    return NULL;
  }
  /*Return pointer to the file*/
  return tmp;
}

/*This function exports the output files*/
void exportFiles(int IC, int DC, char * name)
{
  /*FILE pointers*/
  FILE * object = NULL;
  FILE * entries = NULL;
  FILE * externals = NULL;

  bool externFlag = false;

  wordNode *wordList = NULL;
  symbol *symbolList = NULL;
  entry *entryList = NULL;
  data *dataList = NULL;

  /*open an object file in write mode*/
  object = openFile(name, "w", OB_EXTENSION);
  wordList = getWordsHead();
  printf("\n\t##################EXPORT FILES##################");
  fprintf(object, "%d %d\n", (IC-INTITIAL_IC_VALUE), DC);
  printf("\t###the values of IC and DC are: %d %d\n", (IC-INTITIAL_IC_VALUE), DC);
  /*WORDS*/
  /*go through the words list*/
  while(wordList)
  {
   /*prints commandWord first*/
   /**/
   printf("\t###command name: %s\n", wordList->commandWord.name);
   printf("\t###command era: %c\n", wordList->commandWord.era);
   printf("\t###command is:\n");
   printf("\t%04d %03X %c\n", wordList->commandWord.decimalAddress, (wordList->commandWord.mc & 0xFFF), wordList->commandWord.era);
   /**/
   fprintf(object, "%04d %03X %c\n", wordList->commandWord.decimalAddress, (wordList->commandWord.mc & 0xFFF), wordList->commandWord.era);
   /*checks if source operand exists - if so, we print it data*/
   if(wordList->sourceOperandWord.decimalAddress){
     /**/
     printf("\t###source is:\n");
     printf("\t%04d %03X %c\n", wordList->sourceOperandWord.decimalAddress, (wordList->sourceOperandWord.mc & 0xFFF), wordList->sourceOperandWord.era);
     /**/
     fprintf(object, "%04d %03X %c\n", wordList->sourceOperandWord.decimalAddress, (wordList->sourceOperandWord.mc & 0xFFF), wordList->sourceOperandWord.era);
     /*if there is a value defined as extern we would like to print all the addresses in which it appears*/
     if((!externFlag)&&(wordList->sourceOperandWord.isExternal)){
       externFlag=true;
     }
   }
   printf("\t###check dest\n");
   /*checks if destination operand exists - if so, we print it data*/
   if(wordList->destinationOperandWord.decimalAddress){
     /**/
     printf("\t###dest is:\n");
     printf("\t%04d %03X %c\n", wordList->destinationOperandWord.decimalAddress, (wordList->destinationOperandWord.mc & 0xFFF), wordList->destinationOperandWord.era);
     /**/
     fprintf(object, "%04d %03X %c\n", wordList->destinationOperandWord.decimalAddress, (wordList->destinationOperandWord.mc & 0xFFF), wordList->destinationOperandWord.era);
     if((!externFlag)&&(wordList->destinationOperandWord.isExternal)){
       externFlag=true;
     }
   }
   /*move on to the next command*/
   wordList = wordList->next;
  } /*end of while*/

  /*DATA*/
  /*go through the data list*/
  dataList=getHeadData();
  /* print all the data & addresses in hexadecimal*/
  printf("\t###starts the data\n");
  while (dataList)
  {
    fprintf(object, "%04d %03X %c\n", dataList->address, (dataList->machineCode & 0xFFF), dataList->era);
    printf("\t%04d %03X %c\n", dataList->address, (dataList->machineCode & 0xFFF), dataList->era);
    dataList = dataList->next;
  }

  /*ENTRY*/
  /*go through the entries list*/
  entryList = getEntryHead();
  if(entryList)
  {
    printf("\t###starts the entry\n");
    symbolList = getSymbolHead();
    entries = openFile(name, "w", ENT_EXTENSION);
    while(symbolList)
    {
      if(symbolList->isEntry){
        fprintf(entries, "%s %04d\n", symbolList->label, symbolList->address);
      }
      symbolList = symbolList->next;
    }
  }

  /*EXTERN*/
  /*go through the words list to extract externals address*/
  if(externFlag){
    externals = openFile(name, "w", EXT_EXTENSION);
    wordList = getWordsHead();
    while(wordList){
      if(wordList->sourceOperandWord.isExternal){
        fprintf(externals, "%s %04d\n", wordList->sourceOperandWord.labelName, wordList->sourceOperandWord.decimalAddress);
      }
      if(wordList->destinationOperandWord.isExternal){
        fprintf(externals, "%s %04d\n", wordList->destinationOperandWord.labelName, wordList->destinationOperandWord.decimalAddress);
      }
      wordList = wordList->next;
    }
  }

  printf("\n\t~~~~~~~~~~~~~Success! files exported.~~~~~~~~~~~~~\n");
  /*close all files*/
  if(object)
    fclose(object);
  if(entries)
    fclose(entries);
  if(externals)
    fclose(externals);
}
