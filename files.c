#include "files.h"

#define MASK(x) (x&0xFFF)
#define WORD_PRINT_LINE "%04d %03X %c\n"
#define EXTERNAL_PRINT_LINE "%s %04d\n"

/*internal functions internal declaration*/
FILE *exportObjectFile(char *fileName, bool *hasExternLabel, int IC, int DC);
void exportDataToObjectFile(FILE *objectFilePtr);
void exportEntriesFile(char *fileName);
void exportExternsFile(char *fileName, bool *hasExternLabel);

FILE *openFile(char *filename, char *mode, char *extension)
{
  FILE *currentFile = NULL; /*The file we want to open*/

  /*defines the full name of the file we want to open, including its extension*/
  char fullFileName[MAX_FILENAME_LENGTH];
  strcpy(fullFileName, filename);  /*copy the file name to a dedicated array that will save it*/
  strcat(fullFileName, extension); /*add the file extension*/
  /*try to open the file, and returns value accordingly*/
  if (!(currentFile = fopen(fullFileName, mode)))
  {
    /*couldn't open the file*/
    printf("ERROR: The file \'%s\' could not be opened\n", fullFileName);
    return NULL;
  }
  /*returns a pointer to the file*/
  return currentFile;
}

/*This function gets the file name to create with the 'ob' extension., the flag pointer to update it's value to true if exten label was found and the instruction and data counters.
It exports to a file all the instruction image of the input assembly file. 
It also change the value of the 'hasExternLabel' flag to true if finds an extern declaration*/
FILE *exportObjectFile(char *fileName, bool *hasExternLabel, int IC, int DC)
{
  instructionNode *wordList = NULL;
  FILE *objectFilePtr = openFile(fileName, "w", OB_EXTENSION); /*'w' is a write only mode*/
  if (objectFilePtr)
  {
    wordList = getInsructionsHead();
    fprintf(objectFilePtr, "%d %d\n", IC, DC);
    /*scan through the words list*/
    while (wordList)
    {
      fprintf(objectFilePtr, WORD_PRINT_LINE, wordList->commandWord.decimalAddress, MASK(wordList->commandWord.machineCode), wordList->commandWord.era);
      /*checks if source operand exists - if so, we print it's data*/
      if (wordList->sourceOperandWord.decimalAddress)
      {
        fprintf(objectFilePtr, WORD_PRINT_LINE, wordList->sourceOperandWord.decimalAddress, MASK(wordList->sourceOperandWord.machineCode), wordList->sourceOperandWord.era);
        /*if there is a value defined as extern we would like to print all the addresses in which it appears*/
        if ((!(*hasExternLabel)) && (wordList->sourceOperandWord.isExtern))
        {
          *hasExternLabel = true;
        }
      }
      /*checks if destination operand exists - if so, we print it's data*/
      if (wordList->destinationOperandWord.decimalAddress)
      {
        fprintf(objectFilePtr, WORD_PRINT_LINE, wordList->destinationOperandWord.decimalAddress, MASK(wordList->destinationOperandWord.machineCode), wordList->destinationOperandWord.era);
        /*if there is a value defined as extern we would like to print all the addresses in which it appears*/
        if ((!(*hasExternLabel)) && (wordList->destinationOperandWord.isExtern))
        {
          *hasExternLabel = true;
        }
      }
      wordList = wordList->next; /*move on to the next word*/
    } /*end of while*/
  }
  return objectFilePtr;
}

/*This function gets a pointer to the objects file.
It exports to this file all the data image of the input assembly file*/
void exportDataToObjectFile(FILE *objectFilePtr)
{
  if (objectFilePtr)
  {
    dataNode *dataList = getHeadData();
    while (dataList)
    {
      fprintf(objectFilePtr, WORD_PRINT_LINE, dataList->address, MASK(dataList->machineCode), dataList->era);
      dataList = dataList->next; /*move on to the data word*/
    }                            /*end of while*/
    fclose(objectFilePtr);
  }
}

/*This function gets the file name to create with the 'ent' extension.
It exports to a file all the entries of the input assembly file, if there are none do not create the file.*/
void exportEntriesFile(char *fileName)
{
  entryNode *entryList = getEntryHead(); /*get the pointer to the entries list*/
  /*go through the entries list*/
  if (entryList)
  {
    FILE *entriesFilePtr = openFile(fileName, "w", ENT_EXTENSION);
    symbol *symbolList = getSymbolHead();
    if (entriesFilePtr)
    {
      /*means the file opened successfully*/
      while (symbolList)
      {
        if (symbolList->isEntry)
        {
          fprintf(entriesFilePtr, EXTERNAL_PRINT_LINE, symbolList->label, symbolList->address);
        }
        symbolList = symbolList->next;
      } /*end of while*/
      fclose(entriesFilePtr);
    }
  }
}

/*This function gets the file name to create with the 'ext' extension.
It exports to a file all the externs of the input assembly file, if there are none do not create the file.*/
void exportExternsFile(char *fileName, bool *hasExternLabel)
{
  /*checks if there is an extern label in the assembly file*/
  if (*hasExternLabel)
  {
    FILE *externalsFilePtr = openFile(fileName, "w", EXT_EXTENSION);
    if (externalsFilePtr)
    {
      /*go through the words list to extract externals address*/
      instructionNode *wordList = getInsructionsHead();
      while (wordList)
      {
        if (wordList->sourceOperandWord.isExtern)
        {
          fprintf(externalsFilePtr, EXTERNAL_PRINT_LINE, wordList->sourceOperandWord.labelName, wordList->sourceOperandWord.decimalAddress);
        }
        if (wordList->destinationOperandWord.isExtern)
        {
          fprintf(externalsFilePtr, EXTERNAL_PRINT_LINE, wordList->destinationOperandWord.labelName, wordList->destinationOperandWord.decimalAddress);
        }
        wordList = wordList->next;
      } /*end of while*/
      fclose(externalsFilePtr);
    }
  }
}

/*exports the output files*/
void exportFiles(char *fileName, int IC, int DC)
{
  /*set a pointer that we will use for the object file to add the data*/
  FILE *objectFilePtr = NULL;
  /*use this flag to know if there are extern labels inside the assembly code*/
  bool hasExternLabel = false;
  /*exports the all the data to the appropriate files*/
  objectFilePtr = exportObjectFile(fileName, &hasExternLabel, IC, DC);
  exportDataToObjectFile(objectFilePtr);
  exportEntriesFile(fileName);
  exportExternsFile(fileName, &hasExternLabel);
}
