#include "files.h"

/*This file contains function which handle the errors list*/

/*pointer to head*/
static error *errorHead = NULL;

/*This function adds an error to the errors list.
meant for internal use
HOW TO USE:
addError(err, line, str)
err - string which describes the error. no need to add newline or anything.
line - line number. a negative number means no line.
str - adds an error string (primarily for syntax errors)
*/
void addError(char *err, int line, char *str)
{
  /*create a new error node*/
  error *newError = malloc(sizeof(error));
  checkAllocation(newError);

  /*assign error stringdd*/
  newError->errorMessage = err;
  /*assign error line*/
  newError->line = line;
  /*if we received another string*/
  if (str)
  {
    /*check if last char is newline, if so, remove it*/
    if (str[strlen(str)-1] == '\n')
    {
      str[strlen(str)-1] = 0;
    }
    /*assign the string to str*/
    newError->str = malloc(sizeof(char)*MAX_LINE_LENGTH);
    checkAllocation(newError->str);
    strcpy(newError->str, str);
  }
  /*if we didn't receive another string*/
  else
  {
    /*set str to null*/
    newError->str = NULL;
  }
  /*set next to null*/
  newError->next = NULL;
  if(!errorHead)
  {
    /*point head to temp*/
    errorHead=newError;
  }
  /*if head is not null*/
  else
  {
    error *ptr = errorHead;
    while (ptr->next){
      ptr = ptr->next;
    }
    ptr->next = newError;
  }
}


/*This function will return 0 if there's no error, and another value if there was an error*/
int hasError()
{
  if (errorHead)
    return true;
  else
    return false;
}

/*This function returns the last line an error appears in.
used in the error printing loop*/
int getMaxLine()
{
  error * p;
  int maxLine = 1;
  p = errorHead;
  if (!p)
    return 0;
  while (p)
  {
    if (p->line > maxLine)
      maxLine = p->line;
    p = p->next;
  }
  return maxLine;
}

/*this function will write the errors to an errors file*/
void printErrors(char * filename)
{
  /*error file*/
  FILE * tmp = NULL;
  error * ptr = errorHead;
  int maxLine = getMaxLine();
  int index = 1;
  /*create filename for this input file*/
  char error_filename[MAX_FILENAME];
  strcpy(error_filename, "errors-");
  strcat(error_filename, filename);
  /*pointer to go through all the errors*/

  /*open the file and check if succeeded*/
  if ((tmp = openFile(error_filename, "w", ".txt")))
  {
    if (!ptr)
    {
      fclose(tmp);
      return;
    }
    /*write the errors*/
    while (index <= maxLine)
    {
      ptr = errorHead;
      while (ptr)
      {
        if (ptr->line == index)
        {
          if (ptr->str)
            fprintf(tmp, "ERROR, LINE %d:\t %s \"%s\".\n", ptr->line, ptr->errorMessage, ptr->str);
          else
          {
            fprintf(tmp, "ERROR, LINE %d:\t %s.\n", ptr->line, ptr->errorMessage);
          }
        }
        ptr = ptr->next;
      }
      index++;
    }
    ptr = errorHead;
    while (ptr)
    {
      if (ptr->line < 0)
      {
        if (ptr->str)
          fprintf(tmp, "%s: \"%s\"\n.", ptr->errorMessage, ptr->str);
        else
        {
          fprintf(tmp, "%s.\n", ptr->errorMessage);
        }
      }
      ptr = ptr->next;
    }
    printf("Operation failed. Errors exported to %s.txt\n", error_filename);
  }
  fclose(tmp);

}

/*clean up the errors list*/
void freeErrors()
{
  error *ptr = errorHead;
  if (!ptr){
    return;
  }
  while(ptr)
  {
    error *tmp = ptr;
    ptr = ptr->next;
    free(tmp->str);
    free(tmp);
  }
  errorHead = NULL;
}
