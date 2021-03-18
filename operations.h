#ifndef operations_H
#define operations_H

#include "header.h"
#include "symbolsList.h"
#include "wordsList.h"
#include "utilsFirstPass.h"


/*structures*/
typedef struct instructions
{
  char *name;
  unsigned int opcode:4;
  unsigned int funct:4;
  unsigned int operandCount:2;
} instruction;  


/*functions*/
bool isImmediateAddress(char *token); 
bool isDirective(char *token);
bool isRelative(char *token);
bool isRegister(char *token);
bool isOperation(char *token);
bool isLegalImmediateAddress(char *token, int fileLineNumber, int *num);
void setCommandWord(wordNode *wordNodePtr, instruction *currentInstruction, int *IC);
bool setImmediateOperandWord(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
bool setRegisterOperandWord(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
bool setRelativeOperand(wordNode *wordNodePtr, char *token, int fileLineNumber, int *IC);
bool setLabelOperand(wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
int getTokenType(char *token);
void setOperandWord(bool *errorFlag, wordNode *wordNodePtr, char *token, int fileLineNumber, int isSourceOperand, int *IC);
void analyzeOperation(char *token, int fileLineNumber, int *IC, char *label, bool *errorFlag);


#endif
