#ifndef files_H
#define files_H


#include "header.h"
#include "entryList.h"
#include "dataList.h"
#include "symbolsList.h"
#include "wordsList.h"
#include "utilsFirstPass.h"


/*functions*/
FILE * openFile(char* filename, char * mode, char * extension);
void exportFiles(int IC, int DC, char * name);

#endif
