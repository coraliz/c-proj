#include "header.h"

/*Pointer to head
static wordNode * commandCodeHead = NULL;*/

static wordNode *commandCodeHead = NULL;

/*Add a word node to the list*/
void addWordNode(wordNode *node)
{
  node->next=NULL; /*add it to the end of the linked list*/

  /*If head is null*/
  if (!commandCodeHead)
  {
    /*point head to node*/
    commandCodeHead = node;
  }
  /*If head is not null*/
  else
  {
    /*Go through the list until the last node*/
    wordNode *p = commandCodeHead;
    while (p->next)
      p = p->next;
    /*Assign the last node's next pointer to the new node*/
    p->next = node;
  }
}

/*This function will update all of the words with their corresponding addresses
void updateAddresses()
{
  int address = 100;
  wordNode *ptr = commandCodeHead;
  if (!ptr){
    return;
  }
  while (ptr)
  {
    ptr->address = address;
    address++;
    ptr = ptr->next;
  }
  printf("finished to update all address\n");
} */

/*This function returns the number of nodes in the list*/
int countWords()
{
  int counter = 0;
  wordNode *ptr = commandCodeHead;
  if (!ptr){
    return counter;
  }
  while (ptr)
  {
    counter++;
    ptr = ptr->next;
  }
  return counter;
}

/*This function cleans up the list*/
void freeWords()
{
  wordNode *ptr=commandCodeHead;
  if (!ptr)
  {
    return;
  }
  while (ptr)
  {
    wordNode *tmp=ptr;
    ptr = ptr->next;
    if(tmp->sourceOperandWord.labelName){
      free(tmp->sourceOperandWord.labelName);
      printf("\tfree tmp->sourceOperandWord.labelName\n");
    }
    if(tmp->destinationOperandWord.labelName){
      free(tmp->destinationOperandWord.labelName);
      printf("\tfree tmp->destinationOperandWord.labelName\n");
    }
    free(tmp);
  }
  commandCodeHead = NULL;
  printf("commandCodes is free\n");
}

/*This function will search for a word with a missing label and return it.
if no such word was found, it will return NULL*/
operandWord *getLabelWord()
{
  wordNode *p = commandCodeHead;
  printf("\t##inside get label function\n");
  while (p)
  {
    if((p->sourceOperandWord.labelName)&&(!(p->sourceOperandWord.hasMachineCode))){
      return (&(p->sourceOperandWord));
    }
    else if((p->destinationOperandWord.labelName)&&(!(p->destinationOperandWord.hasMachineCode))){
      return (&(p->destinationOperandWord));
    }
    p=p->next;
  }
  printf("\t##no more labels\n");
  return NULL;
}

/*This function return a pointer to the head of the list*/
wordNode *getWordsHead()
{
  wordNode *tmp;
  tmp = commandCodeHead;
  return tmp;
}
