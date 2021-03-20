#include "firstScan.h"

#define COLON_CHAR ':'
#define COMMENT_FIRST_CHAR ';'
#define DIRECTIVE_FIRST_CHAR '.'

/*The function receives a string which is the current directive in reading the input file, 
the line number and a the pointer to the error flag.
It's set an entry or an extern label if it's written int the right convention, if it isn't - change the 'hasError' value to true.*/
void setExternalLabel(char *directive, int fileLineNumber, bool *hasError)
{
  char *token;
  if (!(token = strtok(NULL, " \t\n")))
  {
    printf("\tERROR: This directive must get a label as a parameter (line %d).\n", fileLineNumber);
    *hasError=true;
    return;
  }
  else
  {
    if (isLegalLabel(token, fileLineNumber))
    {
      if (!strcmp(directive, ENTRY_DIRECTIVE_STR))
      {
        /*this is an entry label*/
        addEntry(token, fileLineNumber);
      }
      else
      {
        /*this is an extern label*/
        addSymbol(token, EXTERNAL_SYMBOL_ADDRESS, false, false, true, false, fileLineNumber, hasError);
      }
    }
    else{
      /*this label is illegal - the specific error was printed during the 'isLegalLabel' function run*/
      *hasError=true;
    }
  }
  if ((token = strtok(NULL, " \t\n")))
  {
    printf("\tERROR: This directive can get only one label as a parameter (line %d).\n", fileLineNumber);
    *hasError=true;
  }
}

bool firstScan(FILE *fp, char *fileName, int *IC, int *DC)
{
  char commandLine[MAX_LINE_LENGTH];
  int fileLineNumber = 0;
  bool hasError = false; /*using this variable we can know if there were any errors during this function run*/

  /*iterete the lines of the file given as input to translate each of its lines into system commands*/
  while (fgets(commandLine, MAX_LINE_LENGTH, fp))
  {
    char *token = NULL;
    char *label = NULL;
    fileLineNumber++;

    /*removing trailing newline character from fgets() input*/
    strtok(commandLine, "\r\n"); 
    /*read the first argumen in the command line - 
    based on the fact that after each command (of any kind) there should be a white space*/
    token = strtok(commandLine, " \t\r\n");

    /*if the line is empty or it's a comment we continue the next line in file*/
    if ((!token) || (*token == COMMENT_FIRST_CHAR))
    {
      /*means this line is empty or a comment line*/
      continue;
    }

    /*check whether the first argument in the line is written in the appropriate convention for the label*/
    if (token[strlen(token) - 1] == COLON_CHAR) 
    {
      /*means it has a label convention*/
      token[strlen(token) - 1] = 0; /*':' is setted to null terminator*/
      if (isLegalLabel(token, fileLineNumber))
      {
        label = token;
        token = strtok(NULL, " \t"); /*reads the input after label declaration*/
        if (!token)
        {
          printf("ERROR: required to write some command after setting the label \'%s\' (file: %s line: %d)", label, fileName, fileLineNumber);
          hasError = true;
          continue;
        }
      }
      else
      {
        /*this label is illegal, so we move on to the next line*/
        /*the specific error was printed during the 'isLegalLabel' function run*/
        hasError = true;
        continue;
      }
    }
    /*check if the command line's token is a directive*/
    if (*token == DIRECTIVE_FIRST_CHAR)
    {
      /*.data*/
      if (!strcmp(token, DATA_DIRECTIVE_STR))
      {
        char *nums = strtok(NULL, "\0");
        if (label)
        {
          addSymbol(label, *DC, false, true, false, false, fileLineNumber, &hasError);
        }
        if (isLegalCommadConvention(nums, fileLineNumber)){
          addNumbers(nums, fileLineNumber, DC, &hasError);
        }
        /*other wise, the specific error was printed during the 'isLegalCommadConvention' function run*/     
      }
      /*.string*/
      else if (!strcmp(token, STRING_DIRECTIVE_STR))
      {
        char *string = strtok(NULL, "\0");
        if (label)
        {
          addSymbol(label, *DC, false, true, false, false, fileLineNumber, &hasError);
        }
        addString(string, fileLineNumber, DC, &hasError);
      }
      /*'.entry' or '.extern'*/
      else if ((!strcmp(token, ENTRY_DIRECTIVE_STR)) || (!strcmp(token, EXTERN_DIRECTIVE_STR)))
      {
        if (label)
        {
          printf("\tWARNING: the label %s before \'%s\' directive is meaningless (line %d).\n", label, token, fileLineNumber);
        }
        setExternalLabel(token, fileLineNumber, &hasError);
      }
      else
      {
        /*has directive convention but unrecognized*/
        printf("\tERROR: \'%s\' is an invalid directive (line %d).\n", token, fileLineNumber);
        hasError = true;
      }
      /*eventually, if it's a directive it continue to the next line (because we used the other function to parss the line)*/
      continue;
    }
    else
    {
      /*otherwise, it is probably a line that contains one of the assembly commands,
      so we will continue to analyze it using a dedicated function*/
      setInstructionWords(token, fileLineNumber, IC, label, &hasError);
    }
  }
  /*If the file is empty*/
  if (fileLineNumber == 0)
  {
    printf("\tERROR: This file \'%s\' is empty.\n", fileName);
  }
  return (!hasError);
}
