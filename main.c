#include "header.h"

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
    printf("No output file was provided.\n");
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
        firstPass(f, &IC, &DC);
        if (hasError()){
          printf("\n\t##finished first pass with errors!##\n");
        }
        printf("\n\t##finished first pass##\n");
        updateDataSymbols(IC);
        updateDataAddresses(IC);
        /*Second pass*/
        secondPass(IC, DC);
        if (hasError()){
          printf("\n\t###########finished second pass with errors!##\n");
        }
        printf("finished the whole running, next is to check if there are errors\n");
        /*If there were no errors, export the files*/
        if (!hasError()){
          exportFiles(IC, DC, argv[index]);
        }
        else{
          printErrors(argv[index]);
          /*Free the errors list*/
          freeErrors();
        }
        /*error file, flag and shit!*/
        /*Cleanup*/
        freeData();
        freeSymbols();
        freeEntries();
        freeWords();
        fclose(f);
      }

    }
  }

  return 0;
}