#ifndef utilsFirstPass_H
#define utilsFirstPass_H

#include "header.h"
#include "dataList.h"
#include "entryList.h"
#include "symbolsList.h"
#include "operations.h"


/*functions*/
void checkAllocation(void *ptr);
int isLablelConvention(char *token);
int isLegalLabel(char *token, int lineNumber);
bool isComment(char *token);
bool isDirectiveConvention(char *token);
bool isLegalCommadConvention(char *commandLine, int fileLineNumber);
void setExternal(char *str, int fileLineNumber, bool *errorFlag);


#endif
