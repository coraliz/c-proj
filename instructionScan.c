#include "instructionScan.h"

/*defines*/
#define RELATIVE_ADDRESS_FIRST_CHAR '%'
#define IMMEDIATE_ADDRESS_FIRST_CHAR '#'

#define BITS_TO_OPCODE 8
#define BITS_TO_FUNCT 4
#define BITS_TO_SOURCE_ADDRESSING_METHOD 2

#define GETS_TWO_OPERANDS 2
#define GETS_SINGLE_OPERAND 1
#define DOES_NOT_GET_OPERANDS 0

/*opcodes strings*/
#define MOV_STR "mov"
#define CMP_STR "cmp"
#define ADD_STR "add"
#define SUB_STR "sub"
#define LEA_STR "lea"
#define JMP_STR "jmp"
#define BNE_STR "bne"
#define PRN_STR "prn"
#define JSR_STR "jsr"

/*internal functions internal declaration*/
bool isLegalImmediateAddress(char *token, int fileLineNumber, int *num);
operandType getTokenType(char *token);
void stopInstructionScan(instructionNode *wordPtr, bool *errorsFlag);
/*The following functions are for defining the code of the different word types generated as a result of the different commands*/
void setCommandWord(instructionNode *wordNodePtr, instruction *currentInstruction, int *IC);
bool setImmediateOperandWord(instructionNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
bool setRegisterOperandWord(instructionNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
bool setRelativeOperand(instructionNode *wordNodePtr, char *token, int fileLineNumber, int *IC);
bool setLabelOperand(instructionNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
void setOperandWord(bool *hasError, instructionNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);

/*The function receives a token from the command line and returns the operand type 
(immediate address, relative address, register or label*/
operandType getTokenType(char *token)
{
  if (*token == IMMEDIATE_ADDRESS_FIRST_CHAR)
  {
    return IMMEDIATE;
  }
  else if (*token == RELATIVE_ADDRESS_FIRST_CHAR)
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

/*Receives a line number token and  integer number to point on the conversed number. 
It checks if the immediate address is set correctly - checks if it starts with the # label and if it is a number that 
is in the allowed range. If it does not it prints an error with the relevant line in the input file and the file name.*/
bool isLegalImmediateAddress(char *token, int fileLineNumber, int *num)
{
  char *immediateStr, *errorptr;
  immediateStr = token;
  immediateStr++;
  if(isspace((int)(*immediateStr))){
    printf("\tERROR: The immediate address should appear immediately after the \'#\' character (line %d).\n", fileLineNumber);
    return false;
  }
  /*the immediate vale defined well, now we try to convert it to number*/
  *num = strtol(immediateStr, &errorptr, DECIMAL_BASE);
  /*TODO: checksyntex maybe it's \0 ? */
  if (*errorptr != '\0')
  {
    printf("\tERROR: \'%s\' is an invalid number (line %d).\n", immediateStr, fileLineNumber);
    return false;
  }
  if (*num > MAX_NUMBER_VALUE || *num < MIN_NUMBER_VALUE)
  {
    printf("\tERROR: The immediate number \'%d\' is out of range (line %d).\n", *num, fileLineNumber);
    return false;
  }
  return true;
}

/*This function gets a pointer to a structure that contains all the words of the current command line (instructionNode), 
the properties of the current command, and the instructions counter. 
Its purpose is to define the instruction command word.*/
void setCommandWord(instructionNode *instructionNodePtr, instruction *currentInstruction, int *IC)
{
  strcpy(instructionNodePtr->commandWord.name, currentInstruction->name);
  instructionNodePtr->commandWord.decimalAddress = *IC;
  instructionNodePtr->commandWord.era = ABSOLUTE;
  instructionNodePtr->commandWord.machineCode |= currentInstruction->opcode << BITS_TO_OPCODE;
  instructionNodePtr->commandWord.machineCode |= currentInstruction->funct << BITS_TO_FUNCT;
  /*those are the values for commands that dont have operands, so I sat it as defualt. 
  otherwise it will change itself durong the code*/
  /*update source addressing method to immediate*/
  instructionNodePtr->commandWord.machineCode |= IMMEDIATE << BITS_TO_SOURCE_ADDRESSING_METHOD;
  /*update destination addressing method to immediate*/
  instructionNodePtr->commandWord.machineCode |= IMMEDIATE;
  (*IC)++;
}

/*Gets a pointer to a structure that contains all the words of the current command line (instructionNode), 
a token from the command line which is an immediate operand, the current line number in the file, 
a flag whose value symbols if it is a source operand and a pointer to IC (instruction counter).
Its purpose is to define an immediate word and return true if succeed, otherwise false.*/
bool setImmediateOperandWord(instructionNode *instructionNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  operandWord *currentOperandWord;
  int immediateAddress;
  char *instructionName = instructionNodePtr->commandWord.name;
  if (isSourceOperand)
  {
    /*only these instructions can get an Immediate address in the source operand: ("mov", "cmp", "add", "sub", "prn")*/
    if (strcmp(MOV_STR, instructionName) && strcmp(CMP_STR, instructionName) && strcmp(ADD_STR, instructionName) && strcmp(SUB_STR, instructionName) && strcmp(PRN_STR, instructionName))
    {
      printf("\tERROR: \'%s\' command can't take an immediate number as a source operand (line %d).\n", instructionName, fileLineNumber);
      return false;
    }
    instructionNodePtr->commandWord.machineCode |= IMMEDIATE << BITS_TO_SOURCE_ADDRESSING_METHOD;
    currentOperandWord = &(instructionNodePtr->sourceOperandWord);
  }
  else
  {
    /*it's a destination operand*/
    /*only these instructions can get an immediate address in the destination operand: ("cmp", "prn")*/
    if (strcmp(CMP_STR, instructionName) && strcmp(PRN_STR, instructionName))
    {
      printf("\tERROR: \'%s\' command can't take an immediate number as a destination operand (line %d).\n", instructionName, fileLineNumber);
      return false;
    }
    /*update destination addressing method to immediate*/
    instructionNodePtr->commandWord.machineCode |= IMMEDIATE; 
    /*update currentOperandWord pointering*/
    currentOperandWord = &(instructionNodePtr->destinationOperandWord);
  }
  if (isLegalImmediateAddress(token, fileLineNumber, &immediateAddress))
  {
    /*takes only the last bits that are relevant to unsigned numeber bit count*/
    currentOperandWord->machineCode = ((unsigned short int)immediateAddress); 
    currentOperandWord->era = ABSOLUTE;
    currentOperandWord->labelName = NULL;
    currentOperandWord->isExtern = false;
    currentOperandWord->isRelative = false;
    currentOperandWord->hasMachineCode = true;
    currentOperandWord->decimalAddress = *IC;
    currentOperandWord->fileLineNumber = fileLineNumber;
    return true;
  }
  /*this immediate number is illegal, the error has been printed*/
  return false;
}

/*Gets a pointer to a structure that contains all the words of the current command line (instructionNode), 
a token from the command line which is an register operand, the current line number in the file, 
a flag whose value symbols if it is a source operand and a pointer to IC (instruction counter).
Its purpose is to define an register word and return true if succeed, otherwise false.*/
bool setRegisterOperandWord(instructionNode *instructionNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  operandWord *currentOperandWord;
  int registerNum;
  char *instructionName = instructionNodePtr->commandWord.name;

  if (isSourceOperand)
  {
    if (!strcmp(LEA_STR, instructionName))
    {
      /*the command is 'lea'*/
      printf("\tERROR: \'%s\' command can't take a register as a source operand (line %d).\n", instructionName, fileLineNumber);
      return false;
    }
    currentOperandWord = &(instructionNodePtr->sourceOperandWord);
    instructionNodePtr->commandWord.machineCode |= REGISTER << BITS_TO_SOURCE_ADDRESSING_METHOD; /*source*/
  }
  /*it's a second operand*/
  else
  {
    /*only these instructions can't get a register address as destination operand : ("jmp", "bne", "jsr")*/
    if ((!strcmp(JMP_STR, instructionName)) || (!strcmp(BNE_STR, instructionName)) || (!strcmp(JSR_STR, instructionName)))
    {
      printf("\tERROR: \'%s\' command can't take a register as a destination operand (line %d).\n", instructionName, fileLineNumber);
      return false;
    }
    instructionNodePtr->commandWord.machineCode |= REGISTER; /*destination*/
    currentOperandWord = &(instructionNodePtr->destinationOperandWord);
  }
  registerNum = (token[1] - '0');
  /*the machine code of the hamster is equal to its serial number in the power of 2*/
  currentOperandWord->machineCode = ((unsigned short int)pow(2, registerNum));
  currentOperandWord->era = ABSOLUTE;
  currentOperandWord->labelName = NULL;
  currentOperandWord->isExtern = false;
  currentOperandWord->isRelative = false;
  currentOperandWord->hasMachineCode = true;
  currentOperandWord->decimalAddress = *IC;
  currentOperandWord->fileLineNumber = fileLineNumber;
  return true;
}

/*Gets a pointer to a structure that contains all the words of the current command line (instructionNode), 
a token from the command line which is an relative operand, the current line number in the file, 
a flag whose value symbols if it is a source operand and a pointer to IC (instruction counter).
Its purpose is to define an relative word and return true if succeed, otherwise false.*/
bool setRelativeOperand(instructionNode *instructionNodePtr, char *token, int fileLineNumber, int *IC)
{
  /*here it's only a destination operand*/
  char *instructionName = instructionNodePtr->commandWord.name;
  char *label = (token+1); /*take off the '%' char*/
  if(isspace((int)*label)){
    printf("\tERROR: The label should appear immediately after the \'%%\' character (line %d).\n", fileLineNumber);
    return false;
  }
  /*only these instructions can get a relative address as destination operand:("jmp", "bne", "jsr")*/
  if ((!strcmp(JMP_STR, instructionName)) || (!strcmp(BNE_STR, instructionName)) || (!strcmp(JSR_STR, instructionName)))
  {
    if (setLabelOperand(instructionNodePtr, label, fileLineNumber, false, IC))
    {
      instructionNodePtr->destinationOperandWord.era = ABSOLUTE;
      instructionNodePtr->destinationOperandWord.isRelative = true;
      instructionNodePtr->destinationOperandWord.isExtern = false;
      /*during the 'setLabelOperand' function run this destination addressing method set to DIRECT (=1), 
      here we increase it by 1 to set it to RELATIVE (=2)*/
      instructionNodePtr->commandWord.machineCode++;
      return true;
    }
    else
    {
      /*this label is illegal, the specific error was printed during the 'setLabelOperand' function run*/
      return false;
    }
  }
  /*This is an order that should not have relative address*/
  printf("\tERROR: \'%s\' gets the relative operand \'%s\' which is not is not allowed for this command (line %d).\n", instructionName, label, fileLineNumber);
  return false;
}

/*Gets a pointer to a structure that contains all the words of the current command line (instructionNode), 
a token from the command line which is an label operand, the current line number in the file, 
a flag whose value symbols if it is a source operand and a pointer to IC (instruction counter).
Its purpose is to define an label word and return true if succeed, otherwise false.*/
bool setLabelOperand(instructionNode *instructionNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  operandWord *currentOperandWord;
  /*all instructions can get operand as direct*/
  if (isSourceOperand)
  {
    /*update source addressing method to direct*/
    instructionNodePtr->commandWord.machineCode |= DIRECT << BITS_TO_SOURCE_ADDRESSING_METHOD;
    currentOperandWord = &(instructionNodePtr->sourceOperandWord);
  }
  else
  {
    /*update destination addressing method to direct*/
    instructionNodePtr->commandWord.machineCode |= DIRECT;
    currentOperandWord = &(instructionNodePtr->destinationOperandWord);
  }

  if (isLegalLabel(token, fileLineNumber))
  {
    currentOperandWord->labelName = (char *)malloc(sizeof(char) * MAX_LABEL_LENGTH);
    checkMemoryAllocation(currentOperandWord->labelName);
    strcpy(currentOperandWord->labelName, token);
    currentOperandWord->hasMachineCode = false;
    currentOperandWord->decimalAddress = *IC;
    currentOperandWord->era = RELOCATABLE;
    currentOperandWord->fileLineNumber = fileLineNumber;
    return true;
  }
  else
  {
    /*illegal label error has been written*/
    return false;
  }
}

/*The following function gets a pointer to the flag whose value symbolizes if run errors have occurred, 
pointer to the words definitions in the current command, pointer to the token from the command line,
the code line number, and a flag whose value symbolizes if it is a source operand, and a pointer to IC. 
Its purpose is to use the above functions and define the operand word. If an error occured it changes the 'hasError' flag to true.*/
void setOperandWord(bool *errorFlag, instructionNode *instructionNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC)
{
  int tokenType = getTokenType(token);
  switch (tokenType)
  {
  case IMMEDIATE:
    if (!setImmediateOperandWord(instructionNodePtr, token, fileLineNumber, isSourceOperand, IC))
    {
      *errorFlag = true;
      return;
    }
    break;

  case REGISTER:
    if (!setRegisterOperandWord(instructionNodePtr, token, fileLineNumber, isSourceOperand, IC))
    {
      *errorFlag = true;
      return;
    }
    break;

  case RELATIVE:
    if (!setRelativeOperand(instructionNodePtr, token, fileLineNumber, IC))
    {
      *errorFlag = true;
      return;
    }
    break;

  case LABEL:
    if (!(setLabelOperand(instructionNodePtr, token, fileLineNumber, isSourceOperand, IC)))
    {
      *errorFlag = true;
      return;
    }
    break;
  } /*end of switch cases*/
  (*IC)++; /*updates IC value because we finished to set one word (from any kind)*/
} /*end of func*/

/*The function gets a pointer to the current instruction that we define and a pointer to the flag of errors.
Its purpose is to release the cell in the allocated memory for the current instruction and change the value of the error flag to true.*/
void stopInstructionScan(instructionNode *wordPtr, bool *errorsFlag)
{
  freeInstruction(wordPtr);
  *errorsFlag = true;
}

void setInstructionWords(char *token, int fileLineNumber, int *IC, char *label, bool *errorFlag)
{
  instruction *currentInstruction;
  char *params, *sourceOperandStr, *destinationOperandStr, *extraOperandStr;
  instructionNode *instructionNodePtr = (instructionNode *)calloc(1, sizeof(instructionNode));

  printf("\n\t#########start to analyzing with the instruction: %s \n", token);
  checkMemoryAllocation(instructionNodePtr);
  currentInstruction=getInstruction(token);
  if(!currentInstruction){
    printf("\tERROR: \'%s\' is an unknown command (line %d).\n", token, fileLineNumber);
    stopInstructionScan(instructionNodePtr, errorFlag);
    return;
  }
  /*otherwise - command is found and we start to analyze*/
  params = strtok(NULL, "\0");
  printf("\t#########params are: %s \n", params); /*TODO: take it off*/
  if (!isLegalCommadConvention(params, fileLineNumber))
  {
    stopInstructionScan(instructionNodePtr, errorFlag);
    return;
  }

  if (label)
  {
    printf("added %s as a label\n", label);
    addSymbol(label, *IC, true, false, false, false, fileLineNumber, errorFlag);
  }
  /*switch case*/
  setCommandWord(instructionNodePtr, currentInstruction, IC);
  switch (currentInstruction->operandCount)
  {
  case GETS_TWO_OPERANDS:
    sourceOperandStr = strtok(params, " ,\t\0");
    destinationOperandStr = strtok(NULL, " ,\t\0");
    extraOperandStr = strtok(NULL, " ,\t\0");

    printf("\n\t###sourceOperandStr is: *%s*\n", sourceOperandStr);
    printf("\n\t###destinationOperandStr is: *%s*\n", destinationOperandStr);
    printf("\n\t###extraOperandStr is: *%s*\n", extraOperandStr);

    if (extraOperandStr)
    {
      printf("\tERROR: This command has more than two parameters (line %d).\n", fileLineNumber);
      stopInstructionScan(instructionNodePtr, errorFlag);
      return;
    }
    else
    {
      if (sourceOperandStr && destinationOperandStr)
      {
        /*set each operand word*/
        setOperandWord(errorFlag, instructionNodePtr, sourceOperandStr, fileLineNumber, true, IC);
        setOperandWord(errorFlag, instructionNodePtr, destinationOperandStr, fileLineNumber, false, IC);
      }
      else
      {
        printf("\tERROR: The command \'%s\' command has less than two parameters (line %d).\n", instructionNodePtr->commandWord.name, fileLineNumber);
        stopInstructionScan(instructionNodePtr, errorFlag);
        return;
      }
    }
    break;

  case GETS_SINGLE_OPERAND:
    destinationOperandStr = strtok(params, " ,\t\0"); /*supposed to find the only the 0*/
    extraOperandStr = strtok(NULL, " ,\t\0");

    printf("\n\t**destinationOperandStr is: *%s*\n", destinationOperandStr);
    printf("\t**extraOperandStr is: *%s*\n", extraOperandStr);
    printf("\t**params is: *%s*\n", params);

    if (extraOperandStr)
    {
      printf("\tERROR: The command \'%s\' has more than one parameter (line %d).\n", instructionNodePtr->commandWord.name, fileLineNumber);
      stopInstructionScan(instructionNodePtr, errorFlag);
      return;
    }
    else
    {
      if (destinationOperandStr)
      {
        setOperandWord(errorFlag, instructionNodePtr, destinationOperandStr, fileLineNumber, false, IC);
      }
      else
      {
        printf("\tERROR: The command \'%s\' did not receive a parameter (line %d).\n", instructionNodePtr->commandWord.name, fileLineNumber);
        stopInstructionScan(instructionNodePtr, errorFlag);
        return;
      }
    }
    break;

  case DOES_NOT_GET_OPERANDS:
    extraOperandStr = strtok(NULL, " ,\t\0");
    if (extraOperandStr)
    {
      printf("\tERROR: The command \'%s\' should not have parameters (line %d).\n", instructionNodePtr->commandWord.name, fileLineNumber);
      stopInstructionScan(instructionNodePtr, errorFlag);
      return;
    }
    break;
  } /*end of switch cases*/
  addInstructionNode(instructionNodePtr); /*adds the new instruction node to it's linked list*/
}