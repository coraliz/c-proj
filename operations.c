#include "operations.h"

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

bool isImmediateAddress(char *token)
{
  if (*token == '#')
  {
    return true;
  }
  return false;
}

bool isRelative(char *token)
{
  if (*token == '%')
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
      }
      return true;
    }
  }
  return false;
}

bool isOperation(char *token)
{
  int i;
  for (i = 0; i < OPCODES_COUNT; i++)
  {
    if (!strcmp(token, instructions[i].name))
    {
      return true;
    }
  }
  return false;
}

bool isDirective(char *token)
{
  char str[MAX_LABEL_LENGTH] = ".";
  strncat(str, token, MAX_LABEL_LENGTH);
  printf("\t###strncat is: %s", str);
  if ((!strcmp(str, DATA_DIRECTIVE_STR)) || (!strcmp(str, STRING_DIRECTIVE_STR)) ||
      (!strcmp(str, ENTRY_DIRECTIVE_STR)) || (!strcmp(str, EXTERN_DIRECTIVE_STR)))
  {
    return true;
  }
  return false;
}

bool isLegalImmediateAddress(char *token, int fileLineNumber, int *num)
{
  char *ptr, *errorptr;
  ptr = token;
  ptr++;

  printf("\t###start the assert legal immediate address\n");
  *num = strtol(ptr, &errorptr, 10);

  /*TODO: checksyntex*/
  if (*errorptr != 0)
  {
    printf("\tERROR: \'%s\' is an invalid number (line %d).\n", ptr, fileLineNumber);
    return false;
  }
  printf("\t###num is not the problem\n");

  if (*num > MAX_NUMBER_VALUE || *num < MIN_NUMBER_VALUE)
  {
    printf("\tERROR: The immediate number \'%d\' is out of range (line %d).\n", *num, fileLineNumber);
    return false;
  }

  printf("\t###finished assert legal immediate address- returns true\n");
  return true;
}

void setCommandWord(wordNode *wordNodePtr, instruction *currentInstruction, int *IC)
{
  strcpy(wordNodePtr->commandWord.name, currentInstruction->name);
  wordNodePtr->commandWord.decimalAddress = *IC;
  wordNodePtr->commandWord.era = ABSOLUTE;
  wordNodePtr->commandWord.mc |= currentInstruction->opcode << 8;
  wordNodePtr->commandWord.mc |= currentInstruction->funct << 4;
  /*those are the values for commands that dont have operands, so I sat it as defualt. 
  otherwise it will change itself durong the code*/
  wordNodePtr->commandWord.mc |= IMMEDIATE << 2; /*source*/
  wordNodePtr->commandWord.mc |= IMMEDIATE;      /*destination*/
  printf("\n\t*************the first commandWord is(before source and dest): %03X\n", wordNodePtr->commandWord.mc);
  printf("\t*************the first commandWord NAME is(before source and dest): %s\n", wordNodePtr->commandWord.name);
  printf("\t*************the first commandWord NAME is(before source and dest): %s\n", wordNodePtr->commandWord.name);
  printf("\t*************the first commandWord ERA is(before source and dest): %c\n", wordNodePtr->commandWord.era);
  (*IC)++;
}

/*immediate operand*/
bool setImmediateOperandWord(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  operandWord *currentOperandWord;
  int immediateAddress;
  char *instructionName = wordNodePtr->commandWord.name;
  printf("\n\t###start immediate(enternal)\n"); /*take it off*/
  if (isSourceOperand)
  {
    /*only these instructions can get an Immediate address: ("mov", "cmp", "add", "sub", "prn")*/
    /*check if can get first operand as immediate*/
    if (strcmp(MOV_STR, instructionName) && strcmp(CMP_STR, instructionName) && strcmp(ADD_STR, instructionName) && strcmp(SUB_STR, instructionName) && strcmp(PRN_STR, instructionName))
    {
      printf("\tERROR: \'%s\' command can't take an immediate number as a source operand (line %d).\n", instructionName, fileLineNumber);
      return false;
    }
    wordNodePtr->commandWord.mc |= IMMEDIATE << 2; /*source*/
    currentOperandWord = &(wordNodePtr->sourceOperandWord);
  }
  /*it's destination operand*/
  else
  {
    printf("this is dest in immediate\n");
    if (strcmp(CMP_STR, instructionName) && strcmp(PRN_STR, instructionName))
    {
      /*if(strcmp(CMP_STR, instructionName)){*/
      printf("\tERROR: \'%s\' command can't take an immediate number as a destination operand (line %d).\n", instructionName, fileLineNumber);
      return false;
    }
    wordNodePtr->commandWord.mc |= IMMEDIATE;
    currentOperandWord = &(wordNodePtr->destinationOperandWord);
  }
  if (isLegalImmediateAddress(token, fileLineNumber, &immediateAddress))
  {
    /*currentOperandWord->mc = (unsigned short int)(immediateAddress & 0xFFF);*/
    currentOperandWord->mc = ((unsigned short int)immediateAddress);
    currentOperandWord->era = ABSOLUTE;
    currentOperandWord->labelName = NULL;
    currentOperandWord->isExternal = false;
    currentOperandWord->isRelative = false;
    currentOperandWord->hasMachineCode = true;
    currentOperandWord->decimalAddress = *IC;
    currentOperandWord->fileLineNumber = fileLineNumber;
    printf("\t###finished immediate(enternal)\n");
    return true;
  }
  /*this immediate number is illegal, the error has been printed*/
  return false;
}

/*register operand*/
bool setRegisterOperandWord(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  operandWord *currentOperandWord;
  int registerNum;
  char *instructionName = wordNodePtr->commandWord.name;

  printf("\n\t###start register(enternal)\n");
  if (isSourceOperand)
  {
    if (!strcmp(LEA_STR, instructionName))
    {
      printf("\tERROR: \'%s\' command can't take a register as a source operand (line %d).\n", instructionName, fileLineNumber);
      return false;
    }
    currentOperandWord = &(wordNodePtr->sourceOperandWord);
    wordNodePtr->commandWord.mc |= REGISTER << 2; /*source*/
  }
  /*it's a second operand*/
  else
  {
    /*if(isInList(instructionName, sourceOperandCanGetRelativeAddressList, NUM_OF_RELAVITVE_OPERANDS)){*/
    /*only these instructions can't get an Immediate address as destination operand : ("jmp", "bne", "jsr")*/
    if ((!strcmp(JMP_STR, instructionName)) || (!strcmp(BNE_STR, instructionName)) || (!strcmp(JSR_STR, instructionName)))
    {
      printf("\tERROR: \'%s\' command can't take a register as a destination operand (line %d).\n", instructionName, fileLineNumber);
      return false;
    }
    wordNodePtr->commandWord.mc |= REGISTER; /*destination*/
    currentOperandWord = &(wordNodePtr->destinationOperandWord);
  }
  /*do for both cases*/
  registerNum = (token[1] - '0');
  currentOperandWord->mc = ((unsigned short int)pow(2, registerNum));
  currentOperandWord->era = ABSOLUTE;
  currentOperandWord->labelName = NULL;
  currentOperandWord->isExternal = false;
  currentOperandWord->isRelative = false;
  currentOperandWord->hasMachineCode = true;
  currentOperandWord->decimalAddress = *IC;
  currentOperandWord->fileLineNumber = fileLineNumber;
  return true;
}

bool setRelativeOperand(wordNode *wordNodePtr, char *token, int fileLineNumber, int *IC)
{
  /*here it's clearly destination operand*/
  char *instructionName = wordNodePtr->commandWord.name;
  char *label = token + 1; /*take off the '#' char*/
  /*only these instructions can get a relative address as destination operand:("jmp", "bne", "jsr")*/
  if ((!strcmp(JMP_STR, instructionName)) || (!strcmp(BNE_STR, instructionName)) || (!strcmp(JSR_STR, instructionName)))
  {
    if (setLabelOperand(wordNodePtr, label, fileLineNumber, false, IC))
    {
      wordNodePtr->destinationOperandWord.era = ABSOLUTE;
      wordNodePtr->destinationOperandWord.isRelative = true;
      wordNodePtr->destinationOperandWord.isExternal = false;
      wordNodePtr->commandWord.mc++; /*TODO: explain*/
      return true;
    }
    else
    {
      /*the specific error was printed during the 'setLabelOperand' function run*/
      return false;
    }
  }
  /*This is an order that should not have relative address*/
  printf("\tERROR: \'%s\' gets the relative operand \'%s\' which is not is not allowed for this command (line %d).\n", instructionName, label, fileLineNumber);
  return false; /*label is ilegal*/
}

bool setLabelOperand(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  operandWord *currentOperandWord;
  printf("\t###start label(enternal)\n");

  /*all instructions can get operand as direct*/
  if (isSourceOperand)
  {
    wordNodePtr->commandWord.mc |= DIRECT << 2; /*source*/
    currentOperandWord = &(wordNodePtr->sourceOperandWord);
  }
  else
  {
    wordNodePtr->commandWord.mc |= DIRECT; /*destination*/
    currentOperandWord = &(wordNodePtr->destinationOperandWord);
  }

  if (isLegalLabel(token, fileLineNumber))
  {
    currentOperandWord->labelName = malloc(sizeof(char) * MAX_LABEL_LENGTH);
    checkAllocation(currentOperandWord->labelName);
    strcpy(currentOperandWord->labelName, token);
    currentOperandWord->hasMachineCode = false;
    currentOperandWord->decimalAddress = *IC;
    currentOperandWord->era = RELOCATABLE;
    currentOperandWord->fileLineNumber = fileLineNumber;
    printf("finished label(enternal)\n");
    return true;
  }
  else
  {
    /*illegal label error has been written*/
    return false;
  }
}

int getTokenType(char *token)
{
  if (isImmediateAddress(token))
  {
    return IMMEDIATE;
  }
  else if (isRelative(token))
  {
    return RELATIVE;
  }
  else if (isRegister(token))
  {
    return REGISTER;
  }
  else
  {
    return LABEL;
  }
}

void setOperandWord(bool *errorFlag, wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  int tokenType = getTokenType(token);

  switch (tokenType)
  {
  case IMMEDIATE:
    printf("\t###got to immediate in switch case\n");
    if (!setImmediateOperandWord(wordNodePtr, token, fileLineNumber, isSourceOperand, IC))
    {
      printf("error occured\n");
      *errorFlag = true;
      return;
    }
    printf("\t###finished immediate in switch case\n");
    break;

  case REGISTER:
    printf("\t###got to register in switch case\n");
    if (!setRegisterOperandWord(wordNodePtr, token, fileLineNumber, isSourceOperand, IC))
    {
      printf("error occured\n");
      *errorFlag = true;
      return;
    }
    printf("\t###finished register in switch case\n");
    break;

  case RELATIVE:
    printf("\t###got to relative in switch case\n");
    if (!setRelativeOperand(wordNodePtr, token, fileLineNumber, IC))
    {
      printf("ERROR RELATIVE\n");
      *errorFlag = true;
      return;
    }
    printf("\t###finished relative in switch case\n");
    break;

  case LABEL:
    printf("\t###got to label in switch case\n");
    /*the defualt method is DIRECT which is relevant to this case*/
    if (!(setLabelOperand(wordNodePtr, token, fileLineNumber, isSourceOperand, IC)))
    {
      printf("not a good label: %s", token);
      *errorFlag = true;
      return;
    }
    printf("\t###finished label in switch case\n");
    break;
  }        /*end of switch cases*/
  (*IC)++; /*updates IC value*/
} /*end of func*/

/*********************************NEW********************************/
void stopAnalyzer(wordNode *wordPtr, bool *errorsFlag)
{
  freeWord(wordPtr);
  *errorsFlag = true;
}

void analyzeOperation(char *token, int fileLineNumber, int *IC, char *label, bool *errorFlag)
{
  int i;
  bool isCommandFound = false;
  instruction *currentInstruction;
  char *params, *sourceOperandStr, *destinationOperandStr, *extraOperandStr;
  wordNode *wordNodePtr = (wordNode *)calloc(1, sizeof(wordNode));

  printf("\n\t#########start to analyzing with the instruction: %s \n", token);
  checkAllocation(wordNodePtr);

  for (i = 0; i < OPCODES_COUNT; i++)
  {
    if (!strcmp(token, instructions[i].name))
    {
      currentInstruction = &instructions[i];
      isCommandFound = true;
    }
  }
  if (!isCommandFound)
  {
    printf("\tERROR: \'%s\' is an unknown command (line %d).\n", token, fileLineNumber);
    stopAnalyzer(wordNodePtr, errorFlag);
    return;
  }
  /*otherwise - command is found and we start to analyze*/
  params = strtok(NULL, "\0");
  printf("\t#########params are: %s \n", params);
  if (!isLegalCommadConvention(params, fileLineNumber))
  {
    stopAnalyzer(wordNodePtr, errorFlag);
    return;
  }

  if (label)
  {
    printf("added %s as a label\n", label);
    addSymbol(label, *IC, true, false, false, false, fileLineNumber, errorFlag);
  }
  /*switch case*/
  setCommandWord(wordNodePtr, currentInstruction, IC);
  printf("Instruction name is: %s\n\n", wordNodePtr->commandWord.name);
  switch (currentInstruction->operandCount)
  {
  case 2:
    printf("start case 2 operandes\n");

    sourceOperandStr = strtok(params, " ,\t\0");
    destinationOperandStr = strtok(NULL, " ,\t\0");
    extraOperandStr = strtok(NULL, " ,\t\0");

    printf("\n\t###sourceOperandStr is: *%s*\n", sourceOperandStr);
    printf("\n\t###destinationOperandStr is: *%s*\n", destinationOperandStr);
    printf("\n\t###extraOperandStr is: *%s*\n", extraOperandStr);

    if (extraOperandStr)
    {
      printf("\tERROR: This command has more than two parameters (line %d).\n", fileLineNumber);
      stopAnalyzer(wordNodePtr, errorFlag);
      return;
      /*break;*/
    }
    else
    {
      if (sourceOperandStr && destinationOperandStr)
      {
        /*set each operand word*/
        setOperandWord(errorFlag, wordNodePtr, sourceOperandStr, fileLineNumber, true, IC);
        setOperandWord(errorFlag, wordNodePtr, destinationOperandStr, fileLineNumber, false, IC);
      }
      else
      {
        printf("\tERROR: The command \'%s\' command has less than two parameters (line %d).\n", wordNodePtr->commandWord.name, fileLineNumber);
        stopAnalyzer(wordNodePtr, errorFlag);
        return;
        /*errorFlag = true;
        break;*/
      }
    }
    printf("\t###finished case 2 operandes\n");
    break;

  case 1:
    printf("\n\t###start case 1 operandes\n");
    /*commandLine = strtok(NULL, "\0");*/

    destinationOperandStr = strtok(params, " ,\t\0"); /*supposed to find the only the 0*/
    extraOperandStr = strtok(NULL, " ,\t\0");

    printf("\n\t**destinationOperandStr is: *%s*\n", destinationOperandStr);
    printf("\t**extraOperandStr is: *%s*\n", extraOperandStr);
    printf("\t**params is: *%s*\n", params);

    if (extraOperandStr)
    {
      printf("\tERROR: The command \'%s\' has more than one parameter (line %d).\n", wordNodePtr->commandWord.name, fileLineNumber);
      stopAnalyzer(wordNodePtr, errorFlag);
      return;
      /*errorFlag = true;
      break;*/
    }
    else
    {
      if (destinationOperandStr)
      {
        setOperandWord(errorFlag, wordNodePtr, destinationOperandStr, fileLineNumber, false, IC);
      }
      else
      {
        printf("\tERROR: The command \'%s\' did not receive a parameter (line %d).\n", wordNodePtr->commandWord.name, fileLineNumber);
        stopAnalyzer(wordNodePtr, errorFlag);
        return;
        /*errorFlag = true;
        break;*/
      }
    }
    break;

  case 0:
    printf("\n\t###start case 0 operandes\n");
    extraOperandStr = strtok(NULL, " ,\t\0");
    if (extraOperandStr)
    {
      printf("\tERROR: The command \'%s\' should not have parameters (line %d).\n", wordNodePtr->commandWord.name, fileLineNumber);
      stopAnalyzer(wordNodePtr, errorFlag);
      return;
    }
    printf("\n\t###the first commandWord is(before source and dest): %03X \n\n", wordNodePtr->commandWord.mc);
    printf("\t###the first commandWord NAME is(before source and dest): %s \n\n", wordNodePtr->commandWord.name);
    printf("\t###the first commandWord ERA is(before source and dest): %c \n\n", wordNodePtr->commandWord.era);
    break;
  } /*end of switch cases*/
  printf("IC is: %d\n", *IC);
  if (wordNodePtr) /*TODO: can take off this if*/
  {
    addWordNode(wordNodePtr);
  }
}