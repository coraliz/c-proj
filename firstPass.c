#include "header.h"
#include "dataList.h"

/*This file contains the first pass function*/

void firstPass(FILE *fp, int *IC, int *DC)
{
  char commandLine[MAX_LINE_LENGTH];
  int fileLineNumber = 0;

  /*rotate the lines of the file given as input to translate each of its lines into system commands*/
  while (fgets(commandLine, MAX_LINE_LENGTH, fp)) /*current line in the file*/
  {
    char *token = NULL;
    char *label = NULL;
    /**/
    fileLineNumber++;

    /*based on the fact that after each command (of any kind) there should be a white space*/
    printf("\n\n\t\t###FIRST PASS###\n");

    /*CHECK THIS!*/
    /*removing trailing newline character from fgets() input*/

    /*
    commandLine[strcspn(commandLine, "\n")] = '\0';
    printf("\n\n\t\t###INDEX IS: %ld", strcspn(commandLine, "\0"));
    */

    /*commandLine[strcspn(commandLine, "\n")] = '\0';*/
    printf("\t###commandLine IS: *%s* \n", commandLine);
    strtok(commandLine, "\r\n");
    printf("\t###commandLine IS: *%s* \n", commandLine);
    /*read the first argumen in the command line*/
    token = strtok(commandLine, " \t\n");
    printf("\t###token IS: *%s* \n", token);
    /*DONE!*/

    /*if the line is empty or it's a comment we continue the next line in file*/
    if ((!token) || (isComment(token)))
    {
      printf("\t###continue\n");
      continue;
    }

    /*check whether the first argument in the line is written in the appropriate convention for the label*/
    if (isLablelConvention(token))
    {
      token[strlen(token) - 1] = 0; /*':' is setted to null terminator*/
      if (isLegalLabel(token, fileLineNumber))
      {
        label = token;
        printf("saved label name (first pass): %s\n", label);
        token = strtok(NULL, " \t");
        printf("\t###TOKEN AFTER LABEL IS: *%s*\n", token);
        if (!token)
        {
          addError("Must add something next to label", fileLineNumber, NULL);
          continue;
        }
      }
      /*this label is invalid so we move on to the next line*/
      else
      {
        continue;
      }
    }
    /*check if the argument in the command line is a directive*/
    if (isDirectiveConvention(token))
    {
      /*.data*/
      if (!strcmp(token, DATA_DIRECTIVE_STR))
      {
        if (label)
        {
          addSymbol(label, *DC, false, true, false, false, fileLineNumber);
        }
        splitNumbers(fileLineNumber, DC); /*take off token*/
      }
      /*.string*/
      else if (!strcmp(token, STRING_DIRECTIVE_STR))
      {
        if (label)
        {
          addSymbol(label, *DC, false, true, false, false, fileLineNumber);
        }
        addString(fileLineNumber, DC);
      }
      /*.entry*/
      else if (!strcmp(token, ENTRY_DIRECTIVE_STR))
      {
        if (label)
        {
          printf("Warning, line %d: label \"%s\" before entry directive is meaningless\n", fileLineNumber, label);
        }
        setExternal(token, fileLineNumber);
      }
      /*.extern*/
      else if (!strcmp(token, EXTERN_DIRECTIVE_STR))
      {
        if (label)
        {
          printf("Warning, line %d: label \"%s\" before external directive is meaningless\n", fileLineNumber, label);
        }
        setExternal(token, fileLineNumber);
      }
      /*undefined directive*/
      else
      {
        /*has directive convention but unrecognized*/
        addError("Invalid directive", fileLineNumber, token);
      }
      /*eventually, if it's a directive it continue to the next line (because we used the other function to parss the line)*/
      continue;
    }
    else
    {
      /*otherwise, it is probably a line that contains one of the assembly commands,
      so we will continue to analyze it using a dedicated function*/
      analyzeOperation(token, fileLineNumber, IC, label);
    }
  }
  /*If the file is empty*/
  if (fileLineNumber == 0)
  {
    addError("ERROR: File is empty.", -1, NULL);
  }
  /*if(isError()){
    return false;
  }
  return true;*/
}
