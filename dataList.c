#include "dataList.h"

/*pointer to the first node*/
static data *dataHead = NULL;

/*This function takes a string as a parameter, and splits it into tokens of whole numbers.
if the string recieved is null, it will add an error. otherwise, it will loop through the string's
tokens and send each one to the addNumber function*/

void addNumbers(char *nums, int line, int *DC, bool *errorFlag)
{
  char *singleNum;
  printf("\n\t##The token is setNumbers are: *%s*\n", nums);

  if (!nums)
  {
    /*add error and return*/
    printf("\tERROR: you must define numbers that are separated by commas after the \'.data\' directive (line %d).\n", line);
    *errorFlag = true;
    return;
  }
  if (!isLegalCommadConvention(nums, line))
  {
    /*a specific error was printed while running the above function*/
    *errorFlag = true;
    return;
  }
  singleNum = strtok(nums, ",");
  printf("\n\t##The singleNum is is: %s\n", singleNum);
  while (singleNum)
  {
    addNumber(singleNum, line, DC, errorFlag);
    singleNum = strtok(NULL, ",");
  }
}

void addNumber(char *number, int line, int *DC, bool *errorFlag)
{
  char *errorPtr = NULL;
  long int tmpNum;

  printf("\n\t***check number: *%s*\n", number);
  /*convert the string to a number*/
  tmpNum = strtol(number, &errorPtr, CONVERTING_BASE);

  /*if strtol returned an error*/
  printf("\terrorPtr: *%s*\n", errorPtr);
  while (*errorPtr != '\0')
  {
    if (isspace((int)*errorPtr))
    {
      errorPtr++;
    }
    else
    {
      /*According to 'strtol' definitions, if there is a value in the pointer (which isn't a white space) then the conversion failed*/
      printf("\tERROR: \'%s\' is an invalid number (line %d).\n", number, line);
      *errorFlag = true;
      return;
    }
  }
  if ((tmpNum < MIN_NUMBER_VALUE) || (tmpNum > MAX_NUMBER_VALUE))
  {
    printf("The number %s is out of range (line %d)\n", number, line);
    *errorFlag = true;
    return;
  }
  addData(((unsigned short int)tmpNum), DC);
}

void addData(unsigned short int value, int *DC)
{
  data *newData;
  newData = (data *)malloc(sizeof(data));
  checkAllocation(newData);
  printf("\n\t###START TO SET DATA: %X###\n", value);

  newData->era = ABSOLUTE;
  /*newData->machineCode = (value & (0xFFF));*/
  newData->machineCode = value;
  printf("\t###SET DATA: %X###\n", newData->machineCode);
  newData->next = NULL;
  if (!dataHead)
  {
    dataHead = newData;
  }
  else
  {
    data *p = dataHead;
    while (p->next)
      p = p->next;
    p->next = newData;
  }
  (*DC)++;
  printf("\t###DATA SETTED###\n");
}

/*This function adds a string to the data list*/
void addString(char *string, int line, int *DC, bool *errorFlag)
{
  printf("\n\tThe string is: *%s*\n", string);
  if ((!string) || (!strcmp(string, "\"")))
  {
    printf("\tERROR: You must define a string after a '.string' directive surrounded by quotation marks (line %d).\n", line);
    *errorFlag = true;
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
    printf("\tERROR: This string does not surrounded by quotation marks (line %d).\n", line);
    *errorFlag = true;
    return;
  }

  /*Loop adding each character to the data list*/
  while (*string != '\0')
  {
    addData((unsigned short int)string[0], DC);
    string++;
  }
  /*add null terminator*/
  addData(0, DC);
}

/*This function updates the data addresses AFTER we have all our operation words*/
void updateDataAddresses(int IC)
{
  /*pointer to head*/
  data *ptr;
  ptr = dataHead;
  /*if head is null, return*/
  if (!ptr)
  {
    return;
  }
  /*go through every node and assign it an address*/
  while (ptr)
  {
    ptr->address = IC;
    /*increment the address to be assigned to the next node*/
    IC++;
    ptr = ptr->next;
  }
  printf("\n\t###finished to update data address");
}

/*Clear the data list*/
void freeData()
{
  data *ptr = dataHead;
  if (!ptr)
  {
    return;
  }
  /*free each data node*/
  while (ptr)
  {
    data *prevPtr = ptr;
    ptr = ptr->next;
    free(prevPtr);
  }
  dataHead = NULL;
  printf("\n\t###data is free\n");
}

/*return pointer to head. used when exporting files*/
data *getHeadData()
{
  data *ptr = dataHead;
  return ptr;
}
