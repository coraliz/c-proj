#include "header.h"
#include "dataList.h"
#include "entryList.h"
#include "symbolsList.h"
#include "wordsList.h"
#include "files.h"
#include "firstPass.h"
#include "secondPass.h"


int main(int argc, char* argv[])
{
  FILE *f = NULL; /*Pointer to input file*/
  /*Index used to loop through the input files*/
  int index;
  /*Line counter*/
  int IC = INTITIAL_IC_VALUE;
  int DC = INTITIAL_DC_VALUE;

  /*Check if there are no arguments*/
  if (argc == 1) /*mean only the program name was gives, without any input file*/
  {
    /*Print error to screen*/
    printf("\tERROR: No passable file received for running the program.\n");
  }
  else
  {
    /*going through each of the files provided to the program*/
    for (index = 1; index < argc; index++)
    {
      printf("%s.as: ", argv[index]);
      /*Tries to open the file and if successful, runs the program*/
      if ((f = openFile(argv[index], "r", AS_EXTENSION)))
      {
        if(firstPass(f, &IC, &DC)){
          printf("\n\n\t######################successful first######################\n");
          updateDataSymbols(IC);
          updateDataAddresses(IC);
          if(secondPass(IC, DC)){
            exportFiles(IC, DC, argv[index]);
            printf("\n\n\t######################successful run######################\n");
          }
          else{
            printf("\n\n\t######################failded second pass######################\n");
          }
        }
        else{
          printf("\n\n\t######################failded first pass######################\n");
        }
        /*Cleanup*/
        freeData();
        freeSymbols();
        freeEntries();
        freeWords();
        fclose(f);
      } /*end the condition that checks if the file can be opened*/
    } /*end of single file scanning*/
  } /*end of files scanning*/
  printf("\n\n\t######################finished main######################");
  return 0;
} /*end of main*/




/*
        firstPass(f, &IC, &DC);
        if (hasError()){
          printf("\n\t###MAIN: finished first pass with errors!##\n");
        }
        printf("\n\t###MAIN: first pass success##\n");
        updateDataSymbols(IC);
        updateDataAddresses(IC);
        //Second pass
        secondPass(IC, DC);
        if (hasError()){
          printf("\t###MAIN: finished second pass with errors!##\n");
        }
        printf("\t###MAIN: finished the whole running, next is to check if there are errors\n");
        //If there were no errors, export the files
        if (!hasError()){
          exportFiles(IC, DC, argv[index]);
        }
        else{
          printErrors(argv[index]);
          //Free the errors list
          freeErrors();
        }        
      }
    }
  }
  return 0;
}*/