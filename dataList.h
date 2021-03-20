#ifndef dataList_H
#define dataList_H

#include "header.h"

/*structures*/
/*using this structure we maintain the data image as a linked list*/
typedef struct dataNodes
{
  char era;
  int address; /*decimal address*/
  unsigned short int machineCode;
  struct dataNodes *next;
} dataNode;

/*functions*/

/*This function receives a string of numbers, the current file line number and a pointer to the data counter and the errors flag.
It defines each number inside the data image and updates DC value. 
If there is an error it prints it to the user screen with the relevant line number in the file and change 'hasError' to true.*/
void addNumbers(char *nums, inputFileLine *fileLine, int *DC, bool *hasError);

/*This function is given an unsigned value and the data counter poiter.
It adds the value to the data list*/
void addData(unsigned short int value, int *DC);

/*This function receives a string, the current file line number and a pointer to the data counter and the errors flag.
It defines each of the string chars in the data image and updates DC value. 
If there is an error it prints it to the user screen with the relevant line number in the file and change 'hasError' to true.*/
void addString(char *string, inputFileLine *fileLine, int *DC, bool *hasError);

/*This function get the instruction counter value.
it responsibles for updating all data addresses according to the final value of the IC*/
void updateDataAddresses(int IC);

/*Frees up all of the data list allocated memory*/
void freeData();

/*The function returns a pointer to the beginning of the linked list of the data image*/
dataNode *getHeadData();

#endif
