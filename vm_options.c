/******************************************************************************
** Student name: 	Sherman Wong
** Student number: 	s3656264
** Course: 			Advanced Programming Techniques - S1 2018
******************************************************************************/

#include "vm_options.h"

/**
 * vm_options.c this is where you need to implement the system handling
 * functions (e.g., init, free, load, save) and the main options for
 * your program. You may however have some of the actual work done
 * in functions defined elsewhere.
 **/

/**
 * Initialise the system to a known safe state. Look at the structure
 * defined in vm_system.h.
 **/
Boolean systemInit(VmSystem * system)
{
	system->itemList = NULL;
	initCoinsArray(system->cashRegister);
	system->stockFileName = NULL;
	system->coinFileName = NULL;
	system->itemList = malloc(sizeof(List));
	
	if(system->itemList == NULL)
		return FALSE;
	else{
		system->itemList->size = 0;
		system->itemList->head = 0;
		return TRUE;
	}
}

/**
 * Free all memory that has been allocated. If you are struggling to
 * find all your memory leaks, compile your program with the -g flag
 * and run it through valgrind.
 **/
void systemFree(VmSystem * system)
{
	free(system);
}

/**
 * Loads the stock and coin data into the system. You will also need to assign
 * the char pointers to the stock and coin file names in the system here so
 * that the same files will be used for saving. A key part of this function is
 * validation. A substantial number of marks are allocated to this function.
 **/
Boolean loadData(
    VmSystem * system, const char * stockFileName, const char * coinsFileName)
{
	if (loadStock(system, stockFileName) == TRUE
    && loadCoins(system, coinsFileName) == TRUE)
		return TRUE;
	else
		return FALSE;
}

/**
 * Loads the stock file data into the system.
 **/
Boolean loadStock(VmSystem * system, const char * fileName)
{
	char line[MAX_LENGTH];
	char * token, *overflow;
	char cash[MAX_DENOMS];
	FILE * sfp;
	Stock * newItem;
	system->stockFileName = fileName;
	if (fileName == NULL) {
		printf("Error - Stock file could not be opened.\n");
		return FALSE;
    }
	system->stockFileName = fileName;
	sfp = fopen(fileName, "r");
	while (fgets(line, MAX_LENGTH, sfp)){
		newItem = malloc(sizeof(Stock));
		token = strtok(line, STOCK_DELIM);
		strcpy(newItem->id, token);
		token = strtok(NULL, STOCK_DELIM);
		strcpy(newItem->name, token);
		token = strtok(NULL, STOCK_DELIM);
		strcpy(newItem->desc, token);
		token = strtok(NULL, STOCK_DELIM);
		strcpy(cash,token);
		token = strtok(NULL, STOCK_DELIM);
		newItem->onHand = strtol(token, &overflow, 10);
		newItem->price.dollars = strtol(strtok(cash, "."), &overflow, 10);
		newItem->price.cents = strtol(strtok(NULL, "."), &overflow, 10);
		if(addtoList(system->itemList, newItem) == FALSE)
			return FALSE;
	};
    fclose(sfp);
    return TRUE;
}

/**
 * Loads the coin file data into the system.
 **/
Boolean loadCoins(VmSystem * system, const char * fileName)
{
	char cash[MAX_DENOMS];
	int denom;
	int quantity;
	FILE * cfp;
	system->coinFileName = fileName;
	cfp = fopen(fileName, "r");
	if(cfp == NULL){
		printf("Error - Coins file could not be opened.\n");
		return FALSE;
	}
	while(fgets(cash,MAX_DENOMS, cfp)){
		denom = strtol(strtok(cash, COIN_DELIM), NULL, 10);
		quantity = strtol(strtok(NULL, COIN_DELIM), NULL, 10);
		denom = getCoinDenom(system->cashRegister, denom);
		system->cashRegister[denom].count += quantity;
	}
    fclose(cfp);
    return TRUE;
}

/**
 * Saves all the stock back to the stock file.
 **/
Boolean saveStock(VmSystem * system)
{
	FILE * sfp;
	Node * current;
	sfp = fopen(system->stockFileName, "w");
	current = system->itemList->head;
	while (current != NULL) {
		fprintf(sfp, "%s%s%s%s%s%s%u.%02u%s%u\n", 
			current->data->id, STOCK_DELIM,
			current->data->name, STOCK_DELIM,
			current->data->desc, STOCK_DELIM,
			current->data->price.dollars, 
			current->data->price.cents,STOCK_DELIM,
			current->data->onHand);
			current = current->next;
	}
	fclose(sfp);
	return TRUE;    
}

/**
 * Saves all the coins back to the coins file.
 **/
Boolean saveCoins(VmSystem * system)
{
	FILE * cfp;
	int denom = 0;
	cfp = fopen(system->coinFileName, "w");
	for (denom = TEN_DOLLARS; denom >= FIVE_CENTS; denom--) {
		fprintf(cfp, "%u,%u\n", getCoinValue(denom),
			system->cashRegister[denom].count);
	}
	fclose(cfp);
	return TRUE;
}

/**
 * This option allows the user to display the items in the system.
 * This is the data loaded into the linked list in the requirement 2.
 **/
void displayItems(VmSystem * system)
{
	printList(system->itemList);
}

/**
 * This option allows the user to purchase an item.
 * This function implements requirement 5 of the assignment specification.
 **/
void purchaseItem(VmSystem * system)
{
	Node * item = NULL;
	int costRemain, denomIn, coinIn;
	int coinsIn[NUM_DENOMS], coinsOut[NUM_DENOMS];
	char id[ID_LEN + EXTRA_SPACES], change[MAX_LENGTH];
	char changeBuffer[CHANGE_BUFFER_SIZE];
	char * bufferEnd;
	printf("Purchase Item\n");
	printf("--------------\n");
	printf("Please enter the id of the item you wish to purchase: ");
	if (getUserInput(id, ID_LEN+EXTRA_SPACES) == FALSE)
		return;
	item = getItemfromID(system->itemList, id);
	itemPrint(item);
	printf("Please make payment - type in value of each note/coin in cents.\n");
	printf("Only coins and $5 and $10 notes are accepted.\n");
	printf("Press enter without entering value to cancel this purchase.\n");
	costRemain = item->data->price.dollars*100 + item->data->price.cents;
	memset(coinsIn, 0, NUM_DENOMS * sizeof(int));
	memset(coinsOut, 0, NUM_DENOMS * sizeof(int));
	while (costRemain > 0){
		printf("Amount owing: $%d.%02d: ", costRemain/100, costRemain%100);
		if (getUserInput(changeBuffer, CHANGE_BUFFER_SIZE) == FALSE)
			return;
		coinIn = strtol(changeBuffer, &bufferEnd, 10);
		if (*bufferEnd != '\0') {
			printf("Error - Please enter an amount in cents.\n");
			continue;
		}
		denomIn = getCoinDenom(system->cashRegister, coinIn);
		if (denomIn != -1) {
			coinsIn[denomIn]++;
			costRemain -= coinIn;
		}
		else
			printf("Error: $%d.%02d is not a valid denomination.\n", 
			coinIn/100, coinIn%100);
		}
	costRemain *= -1;
	insertCoins(system->cashRegister, coinsIn);
	/* Calculate change and return */
	if (calculateChange(system->cashRegister, coinsOut, costRemain) == TRUE) {
		item->data->onHand--;
		coinsToString(change, coinsOut);	
		printf("Thank you. Here is your %s, and your change of $%d.%02d:%s\n",
				item->data->name, costRemain/100, costRemain%100, change); 
		printf("Please come back soon.\n\n");
	} 
	else {
		/* If unable to return correct change, refund coins */
		removeCoins(system->cashRegister, coinsIn); 
		insertCoins(system->cashRegister, coinsOut);
		coinsToString(change, coinsIn);	
		printf("Sorry, the correct change could not be given. Here is a refund:%s\n\n",
                change);
    }
}

void itemPrint(Node * item){
	if (item == NULL){
		printf("Sorry, the requested item cannot be found.\n");
		return;
	}
	if (item->data->onHand == 0){
		printf("Sorry, this item is out of stock.\n");
		return;
	}
	printf("You have selected \"%s - %s\". This will cost you $%u.%02u.\n", 
			item->data->name, item->data->desc, item->data->price.dollars,
			item->data->price.cents);
}

/**
 * You must save all data to the data files that were provided on the command
 * line when the program loaded up, display goodbye and free the system.
 * This function implements requirement 6 of the assignment specification.
 **/
void saveAndExit(VmSystem * system)
{
	/* Save not successful */
	if (saveStock(system) == FALSE) 
		printf("Error - Stock data could not be saved.\n");
	if (saveCoins(system) == FALSE)
		printf("Error - Coin data could not be saved.\n");
	if (saveStock(system) == FALSE || saveCoins(system) == FALSE){
		printf("Program did not exit due to an error.\n");
		return;
	}
	
	/* Save successful */
	if (saveStock(system) == TRUE && saveCoins(system) == TRUE){
		printf("Stock and coins data saved successfully.\n");
		freeList(system->itemList);
		systemFree(system);
	}
}

/**
 * This option adds an item to the system. This function implements
 * requirement 7 of of assignment specification.
 **/
void addItem(VmSystem * system)
{
	Stock * newItem;
	unsigned idNum = 0;
	char newID[ID_LEN + NULL_SPACE], cash[MAX_DENOMS + NULL_SPACE];
	char * cptr;
	Boolean valid;
	printf("Add Item\n");
	printf("-------------\n\n");
	newItem = malloc(sizeof(Stock));
	do {
		idNum++;
		sprintf(newID, "I%04u", idNum);
	} while(getItemfromID(system->itemList, newID) != NULL);
	printf("This new meal item will have the Item id of %s.\n", newID);
	strcpy(newItem->id, newID);
	printf("Enter the item name: ");
	if (getUserInput(newItem->name, NAME_LEN) == FALSE)
		return;
	printf("Enter the item description: ");
	if (getUserInput(newItem->desc, DESC_LEN) == FALSE)
		return;
	valid = FALSE;
	while (valid == FALSE) {
		printf("Enter the price for this item: ");
		if (getUserInput(cash, MAX_DENOMS) == FALSE)
			return;
		newItem->price.dollars = (unsigned) strtol(strtok(cash, "."), &cptr, 10);
		if (*cptr != '\0') {
			printf("Error - Please enter a valid price.\n");
			continue;
		}
		newItem->price.cents = (unsigned) strtol(strtok(NULL, "."), &cptr, 10);
		if (*cptr != '\0') {
			printf("Error - Please enter a valid price.\n");
			continue;
		}
		if ((newItem->price.dollars*100 + newItem->price.cents)%5 != 0) {
			printf("Error - Please enter a valid price.\n");
			continue;
		}
		newItem->onHand = DEFAULT_STOCK_LEVEL;
		valid = TRUE;
    }
	if (addtoList(system->itemList, newItem) == FALSE) {
		printf("Error - New item could not be added.\n");
		return;
	}
	printf("This item \"%s - %s\" has now been added to the menu.\n\n",
			newItem->name,newItem->desc);
}

/**
 * Remove an item from the system, including free'ing its memory.
 * This function implements requirement 8 of the assignment specification.
 **/
void removeItem(VmSystem * system)
{
	char id[ID_LEN + NULL_SPACE];
	char name[NAME_LEN + NULL_SPACE];
	Node * item;
	printf("Remove Item\n");
	printf("-------------\n\n");
	printf("Enter the item id of the item to remove from the menu: ");   
	if (getUserInput(id, ID_LEN+EXTRA_SPACES) == FALSE)
		return;
	if ((item = getItemfromID(system->itemList, id)) == NULL) {
		printf("Error - No such item id exists.\n");
		return;
    }
	strcpy(name, item->data->name);
	if (removefromList(system->itemList, id) == FALSE) {
		printf("Error - Item could not be removed.\n");
		return;
	}
	printf("\"%s - %s\" has been removed from the system.\n\n",
		id, name);
}

/**
 * This option will require you to display the coins from lowest to highest
 * value and the counts of coins should be correctly aligned.
 * This function implements part 4 of requirement 18 in the assignment
 * specifications.
 **/
void displayCoins(VmSystem * system)
{
	printCoins(system->cashRegister);
}

/**
 * This option will require you to iterate over every stock in the
 * list and set its onHand count to the default value specified in
 * the startup code.
 * This function implements requirement 9 of the assignment specification.
 **/
void resetStock(VmSystem * system)
{
	Node * node = system->itemList->head;
	while (node != NULL){
		node->data->onHand = DEFAULT_STOCK_LEVEL;
		node = node->next;
	}
	printf("All stock has been reset to the default level of %u.\n",
		DEFAULT_STOCK_LEVEL);
}

/**
 * This option will require you to iterate over every coin in the coin
 * list and set its 'count' to the default value specified in the
 * startup code.
 * This requirement implements part 3 of requirement 18 in the
 * assignment specifications.
 **/
void resetCoins(VmSystem * system)
{
	Denomination denom;
	for (denom = FIVE_CENTS; denom <= TEN_DOLLARS; denom++)
		system->cashRegister[denom].count = DEFAULT_COIN_COUNT;
	printf("All coins have been reset to the default level of %u.\n", 
		DEFAULT_COIN_COUNT);
}

/**
 * This option will require you to display goodbye and free the system.
 * This function implements requirement 10 of the assignment specification.
 **/
void abortProgram(VmSystem * system)
{
    freeList(system->itemList);
    systemFree(system);
}
