/******************************************************************************
** Student name: 	Sherman Wong
** Student number: 	s3656264
** Course: 			Advanced Programming Techniques - S1 2018
******************************************************************************/

#ifndef VM_COIN_H
#define VM_COIN_H

#include "vm_system.h"

/**
 * The default coin level used when resetting values.
 **/
#define DEFAULT_COIN_COUNT 20

#define COIN_DELIM ","

#define MULTI_BUFFER_SIZE (3 + EXTRA_SPACES)

void initCoinsArray(Coin * cashReg);
void insertCoins(Coin * cashReg, int * coins);
void removeCoins(Coin * cashReg, int * coins);
void coinsToString(char * string, int * coins);
Denomination getCoinDenom(Coin * cashReg, int amount);
unsigned getCoinValue(Denomination denom);
void printCoins(Coin * cashReg);
Boolean calculateChange(Coin * cashReg, int * change, int amount);

#endif
