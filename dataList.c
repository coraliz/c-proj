#include "dataList.h"

#define STRING_END_CHAR 0

static dataNode *dataHead = NULL; /*pointer to the beginning of the data list*/

/*internal functions internal declaration*/
void addNumber(char *number, int fileLineNumber, int *DC, bool *hasError);

/*This function gets a string of a single number, the current file line number and a pointer to the data counter and the errors flag.
It tries to convert the number to an integer, if the conversion is successful it defines it in the data image and updates DC value. 
If there is an error it prints it to the user screen with the relevant line number in the file and change 'hasError' to true.*/
void addNumber(char *number, int fileLineNumber, int *DC, bool *hasError)
{
  char *errorPtr = NULL;
  long int tmpNum;

  /*convert the string to a number*/
  tmpNum = strtol(number, &errorPtr, DECIMAL_BASE);
  /*According to 'strtol' definitions, if there is a value in the pointer (which isn't a white space) then the conversion failed*/
  while (*errorPtr != '\0')
  {
    if (isspace((int)*errorPtr))
    {
      errorPtr++;
    }
    else
    {
      printf("\tERROR: \'%s\' is an invalid number (line %d).\n", number, fileLineNumber);
      *hasError = true;
      return;
    }
  }
  if ((tmpNum < MIN_NUMBER_VALUE) || (tmpNum > MAX_NUMBER_VALUE))
  {
    printf("The number %s is out of range (line %d)\n", number, fileLineNumber);
    *hasError = true;
    return;
  }
  addData(((unsigned short int)tmpNum), DC);
}

void addNumbers(char *nums, int fileLineNumber, int *DC, bool *hasError)
{
  char *singleNum;
  if (!nums)
  {
    printf("\tERROR: you must define numbers that are separated by commas after the \'.data\' directive (line %d).\n", fileLineNumber);
    *hasError = true;
    return;
  }
  /*split numbers to single number*/
  singleNum = strtok(nums, ",");
  while (singleNum)
  {
    addNumber(singleNum, fileLineNumber, DC, hasError);
    singleNum = strtok(NULL, ",");
  }
}

void addData(unsigned short int value, int *DC)
{
  dataNode *newData;
  newData = (dataNode *)malloc(sizeof(dataNode));
  checkMemoryAllocation(newData);

  newData->era = ABSOLUTE;
  newData->machineCode = value;
  newData->next = NULL;
  if (!dataHead)
  {
    dataHead = newData;
  }
  else
  {
    dataNode *p = dataHead;
    while (p->next)
      p = p->next;
    p->next = newData;
  }
  (*DC)++;
}

void addString(char *string, int fileLineNumber, int *DC, bool *hasError)
{
  /*checks whether the string is empty or equal to "*/
  if ((!string) || (!strcmp(string, "\"")))
  {
    printf("\tERROR: You must define a string after a '.string' directive surrounded by quotation marks (line %d).\n", fileLineNumber);
    *hasError = true;
    return;
  }
  /*check if string is surrounded by quotation marks*/
  if ((string[0] == '"') && (string[strlen(string) - 1] == '"'))
  {
    string[strlen(string) - 1] = '\0'; /*change right quotation marks to null terminator*/
    string++;
  }
  else
  {
    /*it's not surrounded by quotation marks*/
    printf("\tERROR: This string does not surrounded by quotation marks (line %d).\n", fileLineNumber);
    *hasError = true;
    return;
  }
  /*adds each of the characters to the data image until it reaches the end of the string*/
  while (*string != '\0')
  {
    addData((unsigned short int)string[0], DC);
    string++;
  }
  /*add null terminator*/
  addData(STRING_END_CHAR, DC);
}

void updateDataAddresses(int IC)
{
  /*pointer to head*/
  dataNode *dataPtr;
  dataPtr = dataHead;

  if (!dataPtr)
  {
    /*if the list is empty, it is irrelevant*/
    return;
  }
  /*goes over each of the node in the list and updates its address according to the IC*/
  while (dataPtr)
  {
    dataPtr->address = IC;
    IC++; /*update IC value*/
    dataPtr = dataPtr->next;
  }
}

void freeData()
{
  dataNode *ptr = dataHead;
  if (!ptr)
  {
    return;
  }
  /*free each data node*/
  while (ptr)
  {
    dataNode *prevPtr = ptr;
    ptr = ptr->next;
    free(prevPtr);
  }
  dataHead = NULL;
}

dataNode *getHeadData()
{
  dataNode *ptr = dataHead;
  return ptr;
}
