#ifndef dataList_H
#define dataList_H

#include "header.h"
#include "utilsFirstPass.h"

/*defines*/
#define CONVERTING_BASE 10

/*structures*/
typedef struct dataNodes
{
  char era;
  int address; /*decimal address*/
  unsigned short int machineCode;
  struct dataNodes *next;
} data;

/*functions*/
void addNumbers(char *nums, int line, int *DC, bool *errorFlag);
void addNumber(char *number, int line, int *DC, bool *errorFlag);
void addData(unsigned short int value, int *DC);
void addString(char *string, int line, int *DC, bool *errorFlag);
void updateDataAddresses(int IC);
void freeData();
data *getHeadData();

#endif
