#ifndef files_H
#define files_H

/*includes*/
#include "header.h"
#include "entriesList.h"
#include "dataList.h"
#include "symbolsList.h"
#include "instructionsList.h"

/*defines*/
#define MAX_FILENAME_LENGTH 1000
/*files extensions*/
#define AS_EXTENSION ".as"
#define OB_EXTENSION ".ob"
#define ENT_EXTENSION ".ent"
#define EXT_EXTENSION ".ext"


/*functions*/

/*This function gets a the file name to open, the writing permissions mode and the file extension.
It tries to open the file name that is accepted in the input after adding the relevant extension to it. 
If successful - returns a pointer to the file, otherwise returns null.*/
FILE * openFile(char* fileName, char *mode, char *extension);

/*This function gets the required file name and the instruction and data counters values.
It exports all the required files (object file, entries file and externs file) after running the program on the input file*/
void exportFiles(char *fileName, int IC, int DC);

#endif