#ifndef secondScan_H
#define secondScan_H

#include "header.h"
#include "scanUtils.h"
#include "entriesList.h"

/*functions*/

/*This function receives the instruction and data counter values. First it checks that there are no more words in the code than the allowed amount reads. 
If it exists it completes the addresses of all the words belonging to the labels and checks that all the entry labels are defined in the code. 
As long as you do not encounter an error - returns true, otherwise returns false*/
bool secondScan(int IC, int DC);

#endif