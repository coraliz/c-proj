#include "scanUtils.h"

/*defines*/
#define REGISTER_STR_LENGTH 2 /*TODO: check this length*/
#define REGISTERS_COUNT 8
#define OPCODES_COUNT 16

/*static*/
/*TODO: legit?*/
static instruction instructions[16] = {
    /*opcodeName, opcode, funct, operandsCount*/
    {"mov", 0, 0, 2},
    {"cmp", 1, 0, 2},
    {"add", 2, 10, 2},
    {"sub", 2, 11, 2},
    {"lea", 4, 0, 2},
    {"clr", 5, 10, 1},
    {"not", 5, 11, 1},
    {"inc", 5, 12, 1},
    {"dec", 5, 13, 1},
    {"jmp", 9, 10, 1},
    {"bne", 9, 11, 1},
    {"jsr", 9, 12, 1},
    {"red", 12, 0, 1},
    {"prn", 13, 0, 1},
    {"rts", 14, 0, 0},
    {"stop", 15, 0, 0}};

static char *registers[8] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/*This function gets a token from the command line and checks if it a defined directive. 
If so, returning true. Otherwise returns false.*/
bool isDirective(char *token)
{
  char str[MAX_LABEL_LENGTH] = ".";
  strncat(str, token, MAX_LABEL_LENGTH);
  if ((!strcmp(str, DATA_DIRECTIVE_STR)) || (!strcmp(str, STRING_DIRECTIVE_STR)) ||
      (!strcmp(str, ENTRY_DIRECTIVE_STR)) || (!strcmp(str, EXTERN_DIRECTIVE_STR)))
  {
    return true;
  }
  return false;
}

bool isRegister(char *token)
{
  int i;
  if (strlen(token) == REGISTER_STR_LENGTH && token[0] == 'r' && isdigit((int)(token[1])))
  {
    for (i = 0; i < REGISTERS_COUNT; i++)
    {
      if (!strcmp(token, registers[i]))
      {
      return true;
      }
    }
  }
  return false;
}

instruction *getInstruction(char *token)
{
  int i;
  for (i = 0; i < OPCODES_COUNT; i++)
  {
    if (!strcmp(token, instructions[i].name))
    {
      return (&instructions[i]);
    }
  }
  return NULL;
}

bool isLegalLabel(char *token, inputFileLine *fileLine)
{
  int i, tokenLength;
  char currentChar;
  tokenLength = strlen(token);
  /*check if first char is letter*/
  if (!isalpha((int)*token))
  {
    VERBOSE_PRINTING(fileLine,( "ERROR: the label \'%s\' must start with a letter\n", token));
    return false;
  }
  else if (tokenLength > MAX_LABEL_LENGTH)
  {
    VERBOSE_PRINTING(fileLine,( "ERROR: the label \'%s\' is a too long string\n", token));
    return false;
  }
  else if (getInstruction(token) || isRegister(token) || isDirective(token))
  {
    VERBOSE_PRINTING(fileLine,( "ERROR: \'%s\' is a reserved label\n", token));
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
        VERBOSE_PRINTING(fileLine,( "ERROR: \'%c\' is an invalid character in this label definition\n", currentChar));
        return false;
      }
    }
    return true;
  }
}

/*command line parse functions*/
/*This function gets a char checks if it a comma. if so, returns true and false otherwise.*/
bool isComma(char c)
{
    if(c==','){
        return true;
    }
    return false;
}

/*This function gets a string and points to its index value. 
Its purpose is to ignore all white spaces until it reaches a real character, the index value is updated to the relevant value.*/
void ignoreWhiteSpaces(int *i, char *str){
  while((str[*i] != '\0')&&(isspace((int)str[*i])))
  {
    (*i)++;
  }
}

/*This function gets a string and points to its index value. 
Its purpose is to ignore all the chars until it reaches any white space, the index value is updated to the relevant value.*/
void ignoreChars(int *i, char *str){
  while((str[*i] != '\0')&&(!isspace((int)str[*i]))&&(!isComma(str[*i])))
  {
    (*i)++;
  }
}

bool isLegalCommadConvention(char *commandLine, inputFileLine *fileLine)
{
  int i = 0;
  bool missingCommaFlag = false;

  if (!commandLine)
  {
    return true;
  }
  /*get th first char*/
  ignoreWhiteSpaces(&i, commandLine);
  if (isComma(commandLine[i]))
  {
    VERBOSE_PRINTING(fileLine, ("ERROR: The command parameters cannot start with a comma\n"));
    return false;
  }
  else{
    /*this is not a comma-countinue to scan the line until we find commas sequence or '\0'*/
    while(commandLine[i]!='\0')
    {
      if(isComma(commandLine[i])){
        if(!missingCommaFlag){
          VERBOSE_PRINTING(fileLine, ("ERROR: A sequence of comma occurrences was found\n"));
          return false;
        }
        else{
          missingCommaFlag=false;
        }
        i++;
      }
      else{
        /*not a comma*/
        if(missingCommaFlag){
          VERBOSE_PRINTING(fileLine, ("ERROR: Missing a comma that will separate the different arguments\n"));
          return false;
        }
        else{
          /*update flag to be true*/
          missingCommaFlag=true;
        }
        ignoreChars(&i, commandLine);
      }
      ignoreWhiteSpaces(&i, commandLine);
    } /*end of while*/
    if(!missingCommaFlag){
      VERBOSE_PRINTING(fileLine, ("ERROR: A command line cannot end with a comma\n"));      
      return false;
    }
    else{
      return true;
    }
  }
}