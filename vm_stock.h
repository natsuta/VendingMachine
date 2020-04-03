/******************************************************************************
** Student name: 	Sherman Wong
** Student number: 	s3656264
** Course: 			Advanced Programming Techniques - S1 2018
******************************************************************************/

#ifndef VM_STOCK_H
#define VM_STOCK_H

#include "vm_coin.h"

/**
 * The default stock level that all new stock should start with and the value
 * to use when restting stock.
 **/
#define DEFAULT_STOCK_LEVEL 20

#define STOCK_DELIM "|"

List * createList();
Node * createNode();
Boolean addtoList(List * list, Stock * item); 
Boolean removefromList(List * list, const char * id);
Node * getItemfromID(List * list, const char * id);
void freeList(List * list);
void printList(List * list);

#endif
