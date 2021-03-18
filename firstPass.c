#include "firstPass.h"

/*This file contains the first pass function*/

bool firstPass(FILE *fp, int *IC, int *DC)
{
  char commandLine[MAX_LINE_LENGTH];
  int fileLineNumber = 0;
  bool errorFlag = false;

  /*rotate the lines of the file given as input to translate each of its lines into system commands*/
  while (fgets(commandLine, MAX_LINE_LENGTH, fp)) /*current line in the file*/
  {
    char *token = NULL;
    char *label = NULL;
    fileLineNumber++;

    printf("\n\t##########################FIRST PASS##########################\n");
    
    /*
    printf("\t###line length is: *%ld*\n", strlen(commandLine));
    if(strlen(commandLine)==MAX_LINE_LENGTH){
      printf("ERROR: This line is too long (line %d)",fileLineNumber);
      continue;
    }
    */
    
    strtok(commandLine, "\r\n"); /*removing trailing newline character from fgets() input*/
    printf("\t###commandLine IS: *%s* \n", commandLine);
    /*read the first argumen in the command line*/
    /*based on the fact that after each command (of any kind) there should be a white space*/
    
    token = strtok(commandLine, " \t\r\n"); 
    printf("\t###token IS: *%s* \n", token);

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
        printf("\t###saved label name (first pass): %s\n", label);
        token = strtok(NULL, " \t");
        printf("\t###TOKEN AFTER LABEL IS: *%s*\n", token);
        if (!token)
        {
          addError("Must add something next to label", fileLineNumber, NULL);
          printf("ERROR: required to write some command after setting the label \'%s\' (line %d)", label, fileLineNumber);
          errorFlag=true;
          continue;
        }
      }
      /*this label is illegal so we move on to the next line*/
      else
      {
        /*the specific error was printed during the 'isLegalLabel' function run*/
        errorFlag=true;
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
          addSymbol(label, *DC, false, true, false, false, fileLineNumber, &errorFlag);
        }
        addNumbers(fileLineNumber, DC, &errorFlag);
      }
      /*.string*/
      else if (!strcmp(token, STRING_DIRECTIVE_STR))
      {
        if (label)
        {
          addSymbol(label, *DC, false, true, false, false, fileLineNumber, &errorFlag);
        }
        addString(fileLineNumber, DC, &errorFlag);
      }
      /*.entry*/
      else if (!strcmp(token, ENTRY_DIRECTIVE_STR))
      {
        if (label)
        {
          printf("\tWARNING: the label %s before entry directive is meaningless (line %d).\n", label, fileLineNumber); 
        }
        setExternal(token, fileLineNumber, &errorFlag);
      }
      /*.extern*/
      else if (!strcmp(token, EXTERN_DIRECTIVE_STR))
      {
        if (label)
        {
          printf("Warning, line %d: label \"%s\" before external directive is meaningless\n", fileLineNumber, label);
        }
        setExternal(token, fileLineNumber, &errorFlag);
      }
      /*undefined directive*/
      else
      {
        /*has directive convention but unrecognized*/
        addError("Invalid directive", fileLineNumber, token);
        printf("\tERROR: \'%s\' is an invalid directive (line %d).\n", token, fileLineNumber);
        errorFlag=true;
      }
      /*eventually, if it's a directive it continue to the next line (because we used the other function to parss the line)*/
      continue;
    }
    else
    {
      /*otherwise, it is probably a line that contains one of the assembly commands,
      so we will continue to analyze it using a dedicated function*/
      analyzeOperation(token, fileLineNumber, IC, label, &errorFlag);
    }
  }
  /*If the file is empty*/
  if (fileLineNumber == 0)
  {
    printf("\tERROR: This file is empty.\n");
    addError("ERROR: File is empty.", -1, NULL);
  }
  return (!errorFlag);
}
