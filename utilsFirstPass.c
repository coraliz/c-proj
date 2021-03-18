#include "utilsFirstPass.h"


/*This function checks if pointer is null*/
void checkAllocation(void *ptr)
{
  if (!ptr)
  {
    printf("The program has quit due to a memory allocation error.\n");
    exit(0);
  }
}

/*This function checks for a label.
if there is no label, it will return NULL.
if there's an illegal label, it will return $.
if there's a legal label, it will return the label.*/

int isLablelConvention(char *token)
{
  if (token[strlen(token) - 1] == ':')
  {
    return true;
  }
  return false;
}

int isLegalLabel(char *token, int lineNumber)
{
  int i, tokenLength;
  char currentChar;
  tokenLength = strlen(token); /*count the '/0' char*/
  /*check if first char is letter*/
  if (!isalpha((int)*token))
  {
    addError("Labels must start with a letter", lineNumber, token);
    printf("\tERROR: the label \'%s\' must start with a letter (line %d)\n", token, lineNumber);
    return false;
  }
  else if (tokenLength > MAX_LABEL_LENGTH)
  {
    addError("label is too long character", lineNumber, token);
    printf("\tERROR: the label \'%s\' is a too long string (line %d)\n", token, lineNumber);
    return false;
  }
  else if (isOperation(token) || isRegister(token) || isDirective(token))
  {
    addError("This label name is reserved", lineNumber, token);
    printf("\tERROR: \'%s\' is a reserved label (line %d)\n", token, lineNumber);
    return false;
  }
  else
  {
    for (i = 0; i < tokenLength; i++)
    {
      currentChar = token[i];
      if (!isalpha(currentChar) && !isdigit(currentChar))
      {
        /* this is not an alpha and not a digit*/
        addError("Invalid character", lineNumber, token);
        printf("\tERROR: \'%c\' is an invalid char (line %d)\n", currentChar, lineNumber);
        return false;
      }
    }
    return true;
  }
}

/*this function checks if token is a comment*/
bool isComment(char *token)
{
  if (*token == ';')
  {
    printf("\t###ITS A COMMENT\n");
    return true;
  }
  else
  {
    return false;
  }
}

/*This function determines if we received a directive.
it will return an enum int of the proper directive, or an error flag
if there's no such directive. if the string doesn't start with ".",
it will return an enum defined as NONE, meaning it is not a directive*/


bool isDirectiveConvention(char *token)
{
  if (*token == '.')
  {
    return true;
  }
  return false;
}

int iscomma(char c)
{
  return c == ',';
}


void ignoreWhiteSpaces(int *i, char *str){
  while((str[*i] != '\0')&&(isspace((int)str[*i])))
  {
    (*i)++;
  }
}

void ignoreChars(int *i, char *str){
  while((str[*i] != '\0')&&(!isspace((int)str[*i]))&&(!iscomma(str[*i])))
  {
    (*i)++;
  }
}


bool isLegalCommadConvention(char *commandLine, int fileLineNumber)
{
  /*# TODO: commas count?*/
  int i = 0;
  bool missingCommaFlag = false;
  printf("\n\t###line: *%s*\n", commandLine);

  if (!commandLine)
  {
    return true;
  }
  /*get th first char*/
  ignoreWhiteSpaces(&i, commandLine);
  if (iscomma(commandLine[i]))
  {
    addError("The command parameters cannot start with a comma", fileLineNumber, NULL);
    printf("\tERROR: The command parameters cannot start with a comma (line %d).\n", fileLineNumber);
    return false;
  }
  else{
    /*this is not a comma-countinue to scan the line until we find commas sequence or '\0'*/
    while(commandLine[i]!='\0')
    {
      if(iscomma(commandLine[i])){
        if(!missingCommaFlag){
          printf("\tERROR: A sequence of comma occurrences was found (line %d).\n", fileLineNumber);
          return false;
        }
        else{
          missingCommaFlag=false;
        }
      }
      else{
        /*not a comma*/
        if(missingCommaFlag){
          printf("\tERROR: Missing a comma that will separate the different arguments (line %d).\n", fileLineNumber);
          return false;
        }
        else{
          missingCommaFlag=true;
        }
      }
      ++i;
      ignoreChars(&i, commandLine);
      ignoreWhiteSpaces(&i, commandLine);
    } /*end of while*/
    if(!missingCommaFlag){
      printf("\tERROR: A command line cannot end with a comma (line %d).\n", fileLineNumber);
      return false;
    }
    else{
      return true;
    }
  }
}
   

void setExternal(char *str, int fileLineNumber, bool *errorFlag)
{
  char *token;
  char *externalLabel;
  /*char *type=(str+1); will looks like .entry ot .external*/
  if (!(token = strtok(NULL, " \t\n")))
  {
    addError("This directive must have a label parameter", fileLineNumber, NULL);
    printf("\tERROR: This directive must get a label as a parameter (line %d).\n", fileLineNumber);
    *errorFlag=true;
    return;
  }
  else
  {
    if (isLegalLabel(token, fileLineNumber))
    {
      externalLabel = token;
      if (!strcmp(str, ENTRY_DIRECTIVE_STR))
      {
        addEntry(externalLabel, fileLineNumber);
        printf("\n\t###added the entry: %s", externalLabel);
      }
      else
      {
        /*this is an extern label*/
        addSymbol(externalLabel, EXTERNAL_SYMBOL_ADDRESS, false, false, true, false, fileLineNumber, errorFlag);
      }
    }
    else{
      /**/
      *errorFlag=true;
    }
  }
  if ((token = strtok(NULL, " \t\n")))
  {
    addError("This directive can only have only one parameter", fileLineNumber, NULL);
    printf("\tERROR: This directive can only get one label as a parameter (line %d).\n", fileLineNumber);
    *errorFlag=true;
  }
}
