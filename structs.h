#ifndef STRUCTS_H
#define STRUCTS_H
/*This file contains */

/*definition of error type. used in the errors list*/
typedef struct errors
{
  char *errorMessage;
  int line;
  char * str;
  struct errors *next;
} error;


#endif
