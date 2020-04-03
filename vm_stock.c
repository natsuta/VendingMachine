/******************************************************************************
** Student name: 	Sherman Wong
** Student number: 	s3656264
** Course: 			Advanced Programming Techniques - S1 2018
******************************************************************************/

#include "vm_stock.h"

/**
 * vm_stock.c this is the file where you will implement the
 * interface functions for managing the stock list.
 **/

/**
 * Some example functions:
 * create list, free list, create node, free node, insert node, etc...
 */

/* Creates and initialises a new list, then returns pointer */
List * createList(){
	List * list = malloc(sizeof(List));
	list->head = NULL;
	list->size = 0;
	return list;
}

/* Creates new node and initialises it */
Node * createNode(){
	Node * node = malloc(sizeof(Node));
	node->data = NULL;
	node->next = NULL;
	return node;   
}

/* Insert item into list */
Boolean addtoList(List * list, Stock * item){
	Node * current = list->head;
	Node * previous = NULL;
	Node * new = createNode();
	if (list == NULL || item == NULL)
		return FALSE;
	new->data = item;
	/* Go through list to place node */
	while(current != NULL && strcmp(item->name, current->data->name) > 0) {
		previous = current;
		current = current->next;
	}
	new->next = current;
	if (previous == NULL)
		list->head = new;
	else
		previous->next = new;
	list->size++;
	return TRUE;
}

/* Remove item from list */
Boolean removefromList(List * list, const char * id){
	Node * current = NULL;
	Node * previous = NULL;
	current = list->head;
	while (current != NULL && (strcmp(current->data->id,id) != 0)) {
		previous = current;
		current = current->next;
    }
	if (current == NULL) 
		return FALSE;
	if (previous == NULL)
		list->head = current->next; 
	else
		previous->next = current->next;	
	list->size--;
	free(current->data);
	free(current);
	return TRUE; 
}

/* Search items using ID, returns pointer or NULL if no item found */
Node * getItemfromID(List * list, const char * id){
	Node * item;
	item = list->head;
	while (item != NULL && (strcmp(item->data->id,id) != 0))
		item = item->next;
	return item;
}

void freeList(List * list){
	Node * current = list->head;
	Node * temp;
	while (current != NULL) {
		temp = current;
		current = current->next;
		free(temp->data);
		free(temp);
	}
	free(list);
}

void printList(List * list){ /* use system instead? */
	Node * current = list->head;
	int idWidth=0, nameWidth=0, stockWidth=strlen("Available"), temp=0;
	while(current != NULL){
		temp = strlen(current->data->id);
		if (idWidth < temp)
			idWidth = temp;
		temp = strlen(current->data->name);
		if (nameWidth < temp)
			nameWidth = temp;
		/* stockWidth is not calculated because why would and 
		how could a vending machine store millions of items?*/
		current = current->next;
	}
	current = list->head;
	printf("Items Menu\n\n");
	printf("%-*s | %-*s | %-*s | %s\n", 
			idWidth,"ID",
			nameWidth,"Name",
			stockWidth,"Available",
			"Price");
	printf("--------------------------------------------------\n");
	while (current != NULL) {
		printf("%-*s | %-*s | %-*u | $ %u.%02u\n", 
				idWidth,current->data->id,
				nameWidth,current->data->name,
				stockWidth,current->data->onHand,
				current->data->price.dollars,
				current->data->price.cents);
		current = current->next;
	}
	printf("\n");
}

