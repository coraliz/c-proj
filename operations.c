#include "header.h"

#define REGISTER_STR_LENGTH 2

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
  printf("*check if register is: %s*\n", token);
  if (strlen(token) == REGISTER_STR_LENGTH && token[0] == 'r' && isdigit((int)(token[1])))
  {
    for (i = 0; i < NUM_OF_REGISTERS; i++)
    {
      if (!strcmp(token, registers[i]))
      {
      }
      return true;
    }
  }
  printf("*This is not a register: %s*\n", token);
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

bool isLegalImmediateAddress(char *token, int fileLineNumber, int *num)
{
  char *ptr, *errorptr;
  ptr = token;
  ptr++;

  printf("start the assert legal immediate address\n");
  *num = strtol(ptr, &errorptr, 10);

  if (*errorptr != 0)
  {
    addError("Invalid number", fileLineNumber, NULL);
    return false;
  }
  printf("num is not the problem\n");
  /*if the number isn't within range*/
  if (*num > MAX_NUM_IMMEDIATE || *num < MIN_NUMBER_IMMEDIATE)
  {
    addError("Immediate number is out of range", fileLineNumber, NULL);
    return false;
  }
  printf("finished assert legal immediate address- returns true\n");
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
  printf("\n\t*************the first commandWord is(before source and dest): %03X \n\n", wordNodePtr->commandWord.mc);
  printf("\n\t*************the first commandWord NAME is(before source and dest): %s \n\n", wordNodePtr->commandWord.name);
  printf("\n\t*************the first commandWord NAME is(before source and dest): %s \n\n", wordNodePtr->commandWord.name);
  printf("\n\t*************the first commandWord ERA is(before source and dest): %c \n\n", wordNodePtr->commandWord.era);
  (*IC)++;
}

/*immediate operand*/
bool setImmediateOperandWord(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  operandWord *currentOperandWord;
  int immediateAddress;
  char *instructionName = wordNodePtr->commandWord.name;
  printf("start immediate(enternal)\n"); /*take it off*/
  if (isSourceOperand)
  {
    /*only these instructions can get an Immediate address: ("mov", "cmp", "add", "sub", "prn")*/
    /*check if can get first operand as immediate*/
    if (strcmp(OPCODE_MOV, instructionName) && strcmp(OPCODE_CMP, instructionName) && strcmp(OPCODE_ADD, instructionName) && strcmp(OPCODE_SUB, instructionName) && strcmp(OPCODE_PRN, instructionName))
    {
      /*if(!(isInList(instructionName, sourceOperandCanGetImmediateAddressList, NUM_OF_IMMEDIATE_OPERANDS))){*/
      printf("This command can't take an immediate number as a first operand\n"); /*take it off*/
      addError("This command can't take an immediate number as a first operand", fileLineNumber, NULL);
      return false;
    }
    wordNodePtr->commandWord.mc |= IMMEDIATE << 2; /*source*/
    currentOperandWord = &(wordNodePtr->sourceOperandWord);
  }
  /*it's destination operand*/
  else
  {
    printf("this is dest in immediate\n");
    if (strcmp(OPCODE_CMP, instructionName) && strcmp(OPCODE_PRN, instructionName))
    {
      /*if(strcmp(OPCODE_CMP, instructionName)){*/
      printf("This command can't take an immediate number as a second operand\n");
      addError("This command can't take an immediate number as a second operand", fileLineNumber, NULL);
      return false;
    }
    wordNodePtr->commandWord.mc |= IMMEDIATE;
    currentOperandWord = &(wordNodePtr->destinationOperandWord);
  }
  printf("start immediate(enternal)\n"); /*take it off*/
  if (isLegalImmediateAddress(token, fileLineNumber, &immediateAddress))
  {
    currentOperandWord->mc = (unsigned short int)(immediateAddress & 0xFFF);
    currentOperandWord->era = ABSOLUTE;
    currentOperandWord->labelName = NULL;
    currentOperandWord->isExternal = false;
    currentOperandWord->isRelative = false;
    currentOperandWord->hasMachineCode = true;
    currentOperandWord->decimalAddress = *IC;
    currentOperandWord->fileLineNumber = fileLineNumber;
    printf("finished immediate(enternal)\n");
    return true;
  }
  printf("this immediate number is illegal, the error has been added\n");
  return false;
}

/*register operand*/
bool setRegisterOperandWord(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  operandWord *currentOperandWord;
  int registerNum;
  char *instructionName = wordNodePtr->commandWord.name;

  printf("start register(enternal)\n");
  if (isSourceOperand)
  {
    if (!strcmp(OPCODE_LEA, instructionName))
    {
      addError("This command can't take a register as a source operand", fileLineNumber, NULL);
      return false;
    }
    wordNodePtr->commandWord.mc |= REGISTER << 2; /*source*/
    currentOperandWord = &(wordNodePtr->sourceOperandWord);
  }
  /*it's a second operand*/
  else
  {
    /*if(isInList(instructionName, sourceOperandCanGetRelativeAddressList, NUM_OF_RELAVITVE_OPERANDS)){*/
    /*only these instructions can't get an Immediate address as destination operand : ("jmp", "bne", "jsr")*/
    if ((!strcmp(OPCODE_JMP, instructionName)) || (!strcmp(OPCODE_BNE, instructionName)) || (!strcmp(OPCODE_JSR, instructionName)))
    {
      addError("This command can't take a register as a destination operand", fileLineNumber, NULL);
      return false;
    }
    wordNodePtr->commandWord.mc |= REGISTER; /*destination*/
    currentOperandWord = &(wordNodePtr->destinationOperandWord);
  }
  /*do for both cases*/
  registerNum = (token[1] - '0');
  currentOperandWord->mc = (unsigned short int)pow(2, registerNum);
  currentOperandWord->era = ABSOLUTE;
  currentOperandWord->labelName = NULL;
  currentOperandWord->isExternal = false;
  currentOperandWord->isRelative = false;
  currentOperandWord->hasMachineCode = true;
  currentOperandWord->decimalAddress = *IC;
  currentOperandWord->fileLineNumber = fileLineNumber;
  printf("finished register(enternal)\n");
  return true;
}

bool setRelativeOperand(wordNode *wordNodePtr, char *token, int fileLineNumber, int *IC)
{
  /*here it's clearly destination operand*/
  char *instructionName = wordNodePtr->commandWord.name;
  char *label = token + 1; /*take off the '#' char*/
  /*if(isInList(instructionName, sourceOperandCanGetRelativeAddressList, NUM_OF_RELAVITVE_OPERANDS)){*/
  /*only these instructions can get a relative address as destination operand:("jmp", "bne", "jsr")*/
  if ((!strcmp(OPCODE_JMP, instructionName)) || (!strcmp(OPCODE_BNE, instructionName)) || (!strcmp(OPCODE_JSR, instructionName)))
  {
    if (setLabelOperand(wordNodePtr, label, fileLineNumber, false, IC))
    {
      wordNodePtr->destinationOperandWord.era = ABSOLUTE;
      wordNodePtr->destinationOperandWord.isRelative = true;
      wordNodePtr->destinationOperandWord.isExternal = false;
      wordNodePtr->commandWord.mc++;
      /* wordNodePtr->commandWord.mc|=RELATIVE; /destination*/
      return true;
    }
  }
  else
  {
    addError("This command can't take a relative as a destination operand", fileLineNumber, NULL);
  }
  return false; /*label is ilegal*/
}

bool setLabelOperand(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  operandWord *currentOperandWord;
  printf("start label(enternal)\n");

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

/**********************************FINISH TO COPY**********************************/

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
    printf("got to immediate in switch case\n");
    if (!setImmediateOperandWord(wordNodePtr, token, fileLineNumber, isSourceOperand, IC))
    {
      printf("error occured\n");
      *errorFlag = true;
    }
    printf("finished immediate in switch case\n");
    break;

  case REGISTER:
    printf("got to register in switch case\n");
    if (!setRegisterOperandWord(wordNodePtr, token, fileLineNumber, isSourceOperand, IC))
    {
      printf("error occured\n");
      *errorFlag = true;
    }
    printf("finished register in switch case\n");
    break;

  case RELATIVE:
    printf("got to relative in switch case\n");
    if (!setRelativeOperand(wordNodePtr, token, fileLineNumber, IC))
    {
      printf("ERROR RELATIVE\n");
      *errorFlag = true;
    }
    printf("finished relative in switch case\n");
    break;

  case LABEL:
    printf("got to label in switch case\n");
    /*the defualt method is DIRECT which is relevant to this case*/
    if (!(setLabelOperand(wordNodePtr, token, fileLineNumber, isSourceOperand, IC)))
    {
      printf("not a good label: %s", token);
      *errorFlag = true;
    }
    printf("finished label in switch case\n");
    break;
  }        /*end of switch cases*/
  (*IC)++; /*updates IC value*/
} /*end of func*/

/*********************************NEW********************************/

void analyzeOperation(char *token, int fileLineNumber, int *IC, char *label)
{
  int i;
  bool errorFlag = false;
  bool isCommandFound = false;
  instruction *currentInstruction;
  char *sourceOperandStr, *destinationOperandStr, *extraOperandStr;
  wordNode *wordNodePtr = (wordNode *)calloc(1, sizeof(wordNode));
  char *testLine1, *testLine2, *testLine3, *params;

  printf("\n\n\t#########start to analyzing with the instruction: %s \n", token);
  checkAllocation(wordNodePtr);

  params = strtok(NULL, "\0");
  printf("\t#########params are: %s \n", params);
  if (!isLegalCommadConvention(params, fileLineNumber))
  {
    errorFlag = true;
    return;
  }

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
    addError("Unknown command", fileLineNumber, token);
    printf("\t#########Unknown command: *%s*\n", token);
    errorFlag = true;
    return;
  }
  /*otherwise - command is found and we start to analyze*/
  if (label)
  {
    printf("added %s as a label\n", label);
    addSymbol(label, *IC, true, false, false, false, fileLineNumber);
  }
  /*switch case*/
  setCommandWord(wordNodePtr, currentInstruction, IC);
  printf("Instruction name is: %s\n\n", wordNodePtr->commandWord.name);
  switch (currentInstruction->operandCount)
  {
  case 2:
    printf("start case 2 operandes\n");
    if (!isLegalCommadConvention(params, fileLineNumber))
    {
      errorFlag = true;
      break;
    }

    sourceOperandStr = strtok(params, ",");
    destinationOperandStr = strtok(NULL, " ,\t\0");
    extraOperandStr = strtok(NULL, " ,\t\0");

    printf("\n\t**sourceOperandStr is: *%s*\n", sourceOperandStr);
    printf("\n\t**destinationOperandStr is: *%s*\n", destinationOperandStr);
    printf("\n\t**extraOperandStr is: *%s*\n", extraOperandStr);

    if (extraOperandStr)
    {
      addError("This command has too many parameters", fileLineNumber, NULL);
      errorFlag = true;
      break;
    }
    else
    {
      if (sourceOperandStr && destinationOperandStr)
      {
        /*set each operand word*/
        setOperandWord(&errorFlag, wordNodePtr, sourceOperandStr, fileLineNumber, true, IC);
        setOperandWord(&errorFlag, wordNodePtr, destinationOperandStr, fileLineNumber, false, IC);
      }
      else
      {
        addError("This command is required to have two parameters separated by a comma", fileLineNumber, NULL);
        errorFlag = true;
        break;
      }
    }
    printf("finished case 2 operandes\n");
    break;

  case 1:
    printf("start case 1 operandes\n");
    /*commandLine = strtok(NULL, "\0");*/

    destinationOperandStr = strtok(params, " ,\t\0"); /*supposed to find the only the 0*/
    extraOperandStr = strtok(NULL, " ,\t\0");

    printf("\t**destinationOperandStr is: *%s*\n", destinationOperandStr);
    printf("\t**extraOperandStr is: *%s*\n", extraOperandStr);
    printf("\t**params is: *%s*\n", params);

    /*printf("\n\t**commandLine is: *%s*\n", token);
      if(!isLegalCommadConvention(token, fileLineNumber)){
        errorFlag=true;
        break;
      }*/

    if (extraOperandStr)
    {
      addError("This command has too many parameters", fileLineNumber, NULL);
      printf("This command has too many parameters\n");
      errorFlag = true;
      break;
    }
    else
    {
      if (destinationOperandStr)
      {
        setOperandWord(&errorFlag, wordNodePtr, destinationOperandStr, fileLineNumber, false, IC);
      }
      else
      {
        addError("This command must get an operand", fileLineNumber, NULL);
        printf("This command must get an operand");
        errorFlag = true;
        break;
      }
    }
    break;

  case 0:
    printf("start case 0 operandes\n");
    extraOperandStr = strtok(NULL, " ,\t\0");
    if (extraOperandStr)
    {
      addError("This command doesn't have parameters", fileLineNumber, NULL);
      errorFlag = true;
    }
    printf("\n\t@@@@the first commandWord is(before source and dest): %03X \n\n", wordNodePtr->commandWord.mc);
    printf("\n\t@@@@the first commandWord NAME is(before source and dest): %s \n\n", wordNodePtr->commandWord.name);
    printf("\n\t@@@@the first commandWord NAME is(before source and dest): %s \n\n", wordNodePtr->commandWord.name);
    printf("\n\t@@@@the first commandWord ERA is(before source and dest): %c \n\n", wordNodePtr->commandWord.era);
    break;
  } /*end of switch cases*/

  printf("starts all the ifs cases in operantions\n");
  if (errorFlag == true)
  {
    printf("error flag is on\n");
    free(wordNodePtr);
    return;
  }

  printf("IC is: %d\n", *IC);
  /*
  if(label){
    printf("added %s as a label\n", label);
    addSymbol(label, firstICValue, true, false, false, false, fileLineNumber);
  }
  */

  if (wordNodePtr)
  {
    addWordNode(wordNodePtr);
  }
  /*
  if(wordNodePtr->commandWord){
    printf("commandWord is not null %d\n", (commandWord!=NULL));
    addWordNode(wordNodePtr);
    printf("commandWord was added\n");
  }
  if(wordNodePtr->sourceOperandWord){
    printf("sourceOperandWord exists\n");
    printf("sourceOperandWord was added\n");
  }
  if(wordNodePtr->destinationOperandWord){ 
    printf("destinationOperandWord was added\n");
  }*/
}