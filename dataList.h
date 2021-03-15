#ifndef dataList_H
#define dataList_H

#include "header.h"

/*dataList.c functions*/
void addNumber(char * number, int line, int * DC);
void splitNumbers(int line, int * DC);
void freeData();
void addString(int line, int *DC);
data * getHeadData();
void updateDataAddresses(int IC);

#endif
