#ifndef instructionScan_H
#define instructionScan_H

#include "header.h"
#include "symbolsList.h"
#include "instructionsList.h"
#include "scanUtils.h"
#include <math.h>

/*functions*/

/*This function receives a token from the command line that should be a string from the commanf line, 
the current line number in the output code, a pointer to the instruction counter, a string of the label found in the first scan and a flag symbolizing whether there are run errors or not
Its purpose is to define all the code words according to the current command word and its operands. 
If there is an error change the value of the flag to true*/
void setInstructionWords(char *token, int fileLineNumber, int *IC, char *label, bool *hasError);

#endif
