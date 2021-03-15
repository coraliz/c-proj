#include "header.h"

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
    return false;
  }
  else if (tokenLength > MAX_LABEL_LENGTH)
  {
    addError("label is too long character", lineNumber, token);
    return false;
  }
  else if (isOperation(token) || isRegister(token))
  {
    addError("This label name is reserved", lineNumber, token);
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
        printf("\t###Invalid character: %c\n", currentChar);
        addError("Invalid character", lineNumber, token);
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
    printf("\t##ITS A COMMENT\n");
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
directiveType getDirectiveType(char *token)
{
  /*if first char is '.'*/
  if (*token == '.')
  {
    /*return enum corresponding to the directive*/
    if (!strcmp(token, DATA_DIRECTIVE_STR))
    {
      return DATA_DIRECTIVE;
    }
    else if (!strcmp(token, STRING_DIRECTIVE_STR))
    {
      return STRING_DIRECTIVE;
    }
    else if (!strcmp(token, ENTRY_DIRECTIVE_STR))
    {
      return ENTRY_DIRECTIVE;
    }
    else if (!strcmp(token, EXTERN_DIRECTIVE_STR))
    {
      return EXTERN_DIRECTIVE;
    }
    else
    {
      /*return error*/
      return UNDEFINED_DIRECTIVE;
    }
  }
  /*if this isn't a directive*/
  else
  {
    return NOT_A_DIRECTIVE;
  }
}

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

bool isLegalCommadConvention(char *commandLine, int fileLineNumber)
{
  /*# TODO: commas count?*/
  int i = 0;
  int commaCount = 0;
  bool commaFlag = false;
  printf("\t###line: *%s*\n", commandLine);

  if (!commandLine)
  {
    return true;
  }
  /*get th first char*/
  while (isspace((int)commandLine[i]) && commandLine[i] != '\n')
  {
    ++i;
  }
  /*if the first char is a comma*/
  if (iscomma(commandLine[i]))
  {
    addError("The command parameters cannot start with a comma", fileLineNumber, NULL);
    printf("\t###cannot have comma at the begining: %d\n", fileLineNumber);
    return false;
  }
  /*this is not a comma-countinue to sacen the line until we find commas sequence or '\n'*/
  for (; i < (strlen(commandLine)); i++)
  {
    if (iscomma(commandLine[i]))
    {
      ++commaCount;
      if (commaFlag)
      {
        addError("Consecutive commas were found", fileLineNumber, NULL);
        printf("\t###too many commas on line: %d\n", fileLineNumber);
        return false;
      }
      printf("\t###commaFlag is ture\n");
      commaFlag = true;
    }
    /*else if(isalpha((int)commandLine[i])||isdigit((int)commandLine[i])){*/
    else if (!isspace((int)commandLine[i]))
    {
      printf("\t###commaFlag is false: *%c**%d*\n", commandLine[i], i);
      commaFlag = false;
    }
  }
  printf("\t###is here: *%c**%d*\n", commandLine[i], i);
  if (commaFlag)
  {
    addError("Consecutive commas were found", fileLineNumber, NULL);
    printf("\t###too many commas on line: %d\n", fileLineNumber);
    return false;
  }
  /*if(commaCount==0){
    addError("Commas are required between each of the command operands", fileLineNumber, NULL);
    printf("\t###Commas are required between each of the command operands %d", commaCount);
    return false;    
  }*/
  printf("\t###commaCount %d\n", commaCount);
  printf("\t###good line\n");
  return true;
}

void setExternal(char *str, int fileLineNumber)
{
  char *token;
  char *externalLabel;
  /*char *type=(str+1); will looks like .entry ot .external*/
  if (!(token = strtok(NULL, " \t\n")))
  {
    addError("This directive must have a label parameter", fileLineNumber, NULL);
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
        printf("\n\t@@@added the entry: %s", externalLabel);
      }
      else
      {
        /*this is an extern label*/
        addSymbol(externalLabel, EXTERNAL_SYMBOL_ADDRESS, false, false, true, false, fileLineNumber);
      }
    }
  }
  if ((token = strtok(NULL, "\n")))
  {
    addError("This directive can only have only one parameter", fileLineNumber, NULL);
    return;
  }
}
