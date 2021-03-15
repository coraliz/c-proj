/* #include "header.h"*/
#include "dataList.h"
/*This file contains functions which handle the data list*/

/*pointer to the first node*/
static data *dataHead = NULL;


/*This function takes a string as a parameter, and splits it into tokens of whole numbers.
if the string recieved is null, it will add an error. otherwise, it will loop through the string's
tokens and send each one to the addNumber function*/

void splitNumbers(int line, int * DC)
{
  char *nums;
  char *singleNum;

  nums = strtok(NULL, "\0");
  printf("\n\t##The token is splitNumbers is: %s\n", nums);

  if(!nums){
    /*add error and return*/
    addError("You must specify numbers", line, NULL);
    printf("\tYou must specify numbers\n");
    return;
  }
  if(!isLegalCommadConvention(nums, line)){
    printf("\tisn't legal commad convention\n");
    return;
  }
  singleNum=strtok(nums, ", \t\0");
  printf("\n\t##The singleNum is is: %s\n", singleNum);
  while(singleNum){
    addNumber(singleNum, line, DC);
    singleNum=strtok(NULL, " ,\t\0");
  }
}


void addNumber(char *number, int line, int * DC)
{
  char *errorPtr = NULL;
  long int tmpNum;

  printf("\n\t***check number: %s*\n", number);
  /*convert the string to a number*/
  tmpNum = strtol(number, &errorPtr, 10);
  printf("\n\t***1. add number: %ld\n", tmpNum);

  /*if strtok returned an error*/
  if (*errorPtr != '\0')
  {
    /*add an error and exit function*/
    addError("Invalid number", line, number);
    printf("Invalid number\n");
    return;
  }

  /*if number is out of 15-bit range*/
  if ((tmpNum < MIN_NUMBER_DATA) || (tmpNum > MAX_NUMBER_DATA))
  {
    /*add error and exit function*/
    addError("Number is out of range", line, number);
    printf("Number is out of range\n");
    return;
  }
  printf("\n\t***2. add number: %ld\n", tmpNum);
  addData(((unsigned short int)tmpNum), DC);
}


void addData(unsigned short int value, int *DC){
  data *newData;
  newData = (data *)malloc(sizeof(data));
  checkAllocation(newData);
  printf("\t###START TO SET DATA: %X###\n", value);

  newData->era=ABSOLUTE;
  newData->machineCode = (value & (0xFFF));
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
void addString(int line, int *DC)
{
  char *string;
  string = strtok(NULL, "\0");
  printf("\tThe string is: *%s*\n", string);
  printf("\tFirst char is: *%c*\n", string[0]);
  printf("\tLast char is: *%c*\n", string[strlen(string) - 1]);

  if (!string)
  {
    addError("Must specify a string", line, NULL);
    return;
  }
  if (!strcmp(string, "\""))
  {
    addError("Must specify a string", line, NULL); /*if we only recieved a "*/
    return;
  }

  if ((string[0] == '"') && (string[strlen(string) - 1] == '"')) /*Check if string is surrounded by quotation marks*/
  {
    string[strlen(string) - 1] = '\0'; /*change right quotation marks to null terminator*/
    string++;
  }
  /*if it's not surrounded by quotation marks*/
  else
  {
    /*add error and return*/
    addError("Strings must be surrounded by quotation marks", line, NULL);
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
  data * ptr;
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
  printf("finished to update data address");
}
/*Clear the data list*/
void freeData()
{
  data *ptr = dataHead;
  if (!ptr){
    return;
  }
  /*free each data node*/
  while(ptr)
  {
    data *prevPtr = ptr;
    ptr = ptr->next;
    free(prevPtr);
  }
  dataHead = NULL;
  printf("data is free\n");
}

/*return pointer to head. used when exporting files*/
data * getHeadData()
{
  data *ptr = dataHead;
  return ptr;
}
