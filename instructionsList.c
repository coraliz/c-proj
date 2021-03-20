#include "instructionsList.h"

static instructionNode *instructionHead = NULL;

void addInstructionNode(instructionNode *node)
{
  node->next = NULL; 
  if (!instructionHead)
  {
    instructionHead = node;
  }
  else
  {
    instructionNode *tmp = instructionHead;
    while (tmp->next)
      tmp = tmp->next;
    tmp->next = node;
  }
}

void freeInstructions()
{
  instructionNode *ptr = instructionHead;
  if (!ptr)
  {
    return;
  }
  while (ptr)
  {
    instructionNode *tmp = ptr;
    ptr = ptr->next;
    if (tmp->sourceOperandWord.labelName)
    {
      free(tmp->sourceOperandWord.labelName);
    }
    if (tmp->destinationOperandWord.labelName)
    {
      free(tmp->destinationOperandWord.labelName);
    }
    free(tmp);
  }
  instructionHead = NULL;
}

void freeInstruction(instructionNode *instructionNodePtr)
{
  if (instructionNodePtr->sourceOperandWord.labelName)
  {
    free(instructionNodePtr->sourceOperandWord.labelName);
  }
  if (instructionNodePtr->destinationOperandWord.labelName)
  {
    free(instructionNodePtr->destinationOperandWord.labelName);
  }
  free(instructionNodePtr);
}

operandWord *getUndefinedLabelWord()
{
  instructionNode *tmp = instructionHead;
  while (tmp)
  {
    if ((tmp->sourceOperandWord.labelName) && (!(tmp->sourceOperandWord.hasMachineCode)))
    {
      return (&(tmp->sourceOperandWord));
    }
    else if ((tmp->destinationOperandWord.labelName) && (!(tmp->destinationOperandWord.hasMachineCode)))
    {
      return (&(tmp->destinationOperandWord));
    }
    tmp = tmp->next;
  }
  return NULL;
}

instructionNode *getInsructionsHead()
{
  instructionNode *tmp = instructionHead;
  return tmp;
}
