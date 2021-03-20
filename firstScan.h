#ifndef firstScan_H
#define firstScan_H

#include "header.h"
#include "dataList.h"
#include "symbolsList.h"
#include "entriesList.h"
#include "instructionScan.h"

/*This function gets the input file and it's name which we are required to scan. 
Also, receives the counters ​​of the instructions and data.
It's purpose is to make an initial scan on the file to buil the table of symbols and the foundation of the total memory image.
Returns true if no errors were detected, otherwise returns false.*/
bool firstScan(FILE *fp, char *fileName, int *IC, int *DC);

#endif
