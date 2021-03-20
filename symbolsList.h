#ifndef symbolList_H
#define symbolList_H

#include "header.h"

/*structures*/
/*This structure represents the table of symbols, for each defined word: its name, its address in the command line, 
and its type (extern\code\data)*/
typedef struct symbols
{
  char *label;
  short int address;
  bool isExtern;
  bool isEntry;
  bool isCode;
  bool isData;
  struct symbols *next;
} symbol; 

/*functions*/
/*Gets the name of the labels, its address in memory, its type(entry|extern|code|data), 
the relevant line of code in which it is written and a pointer to the error flag.
Its purpose is to define the new icon and add it to the list of icons. if an error occured, changes the error flag to true*/
void addSymbol(char *label, int address, int isCode, int isData, int isExtern, int isEntry, int lineNumber, bool *errorFlag);

/*Receives the final value of the instruction counter. 
Its purpose is to update the address of all the icons in memory belonging to the data image according to the value of the counter.*/
void updateDataSymbols(int IC);

/*Frees up all of the symbol list allocated memory*/
void freeSymbols();

/*This function is given a specific label name and the line that appears in the code. 
Its purpose is to check if the icon already exists in the icon table.
it so, then returns true and prints an error on the user screen. Otherwise returns fasle.*/
bool doesSymbolExist(char *label, int fileLineNumber);

/*
This function gets a label name and checks whether it appears in the table of symbols or not. 
If so returns it, otherwise returns null*/
symbol * searchSymbol(char * label);

/*The function returns a pointer to the beginning of the symbols linked list*/
symbol *getSymbolHead();

#endif
