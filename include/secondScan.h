#ifndef secondScan_H
#define secondScan_H

#include "header.h"
#include "scanUtils.h"
#include "entriesList.h"

/*functions*/

/*This function completes the addresses of all the words belonging to the labels and checks that all the entry labels are defined in the code. 
As long as you do not encounter an error - returns true, otherwise returns false*/
bool secondScan();

#endif