#include "header.h"
#include "dataList.h"
#include "entriesList.h"
#include "symbolsList.h"
#include "instructionsList.h"
#include "files.h"
#include "firstScan.h"
#include "secondScan.h"

void removeFileWithExtension(char* fileName, char *fileExtension){
  /*we don't care if the file deletion was successful*/
  char fullFileName[MAX_FILENAME_LENGTH];
  strcpy(fullFileName, fileName);
  remove(strcat(fullFileName,fileExtension));
}


int main(int argc, char *argv[])
{
  FILE *currentFile = NULL;   /*set the input file pointer*/
  int fileIndex;              /*set current input file index*/
  int IC = INITIAL_IC_VALUE; /*set code instruction counter with it's initial value*/
  int DC = INITIAL_DC_VALUE; /*set data directive counter with it's initial value*/

  /*check if we have received files to run the program on*/
  if (argc == 1)
  {
    /*means only the program name was gives, without any input file*/
    printf("\tERROR: No input file received for running the program.\n");
  }
  else
  {
    /*now we will now run the program on each of the input file*/
    for (fileIndex = 1; fileIndex < argc; fileIndex++)
    {
      /*
      char fullFileName[MAX_FILENAME_LENGTH];
      strcpy(fullFileName, argv[fileIndex]); //copy the filename from argv to the full filename string
      strcat(fullFileName, AS_EXTENSION);    //add the .as extension to the full filename.
      printf("\n\t###start to scan the file: %s###\n", fullFileName);
      */
      /*Tries to open the file and if successful, runs the program*/
      if ((currentFile = openFile(argv[fileIndex], "r", AS_EXTENSION)))
      {
        /*remove any remains of previous executions*/
        removeFileWithExtension(argv[fileIndex], OB_EXTENSION);
        removeFileWithExtension(argv[fileIndex], ENT_EXTENSION);
        removeFileWithExtension(argv[fileIndex], EXT_EXTENSION);
        /*we will continue to run the code only if the first scan was successful, 
        otherwise we have code errors*/
        if (firstScan(currentFile, argv[fileIndex], &IC, &DC))
        {
          /*add the value of IC for the existing values ​​in the data image*/
          updateDataSymbols(IC);
          updateDataAddresses(IC);
          /*we will export all of our information to files only if the second scan was successful, 
          otherwise we have code errors*/
          if (secondScan(IC, DC))
          {
            exportFiles(argv[fileIndex], (IC - INITIAL_IC_VALUE), DC); /* did IC-INTITIAL_IC_VALUE to get the instructions count*/
          }
        }
        /*release all the structures we used during the program run*/
        freeData();
        freeSymbols();
        freeEntries();
        freeInstructions();
        fclose(currentFile);
      } /*end the condition that checks if the file can be opened*, if it won't it continue the next file*/
    }   /*end of single file scanning*/
  }     /*end of files scanning*/
  return 0;
}