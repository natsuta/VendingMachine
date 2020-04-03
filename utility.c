/******************************************************************************
** Student name: 	Sherman Wong
** Student number: 	s3656264
** Course: 			Advanced Programming Techniques - S1 2018
******************************************************************************/

#include "utility.h"

/**
 * Function required to be used when clearing the buffer. It simply reads
 * each char from the buffer until the buffer is empty again. Please refer
 * to the materials on string and buffer handling in the course for more
 * information.
 **/
void readRestOfLine()
{
    int ch;
    while(ch = getc(stdin), ch != EOF && ch != '\n')
    { } /* Gobble each character. */

    /* Reset the error status of the stream. */
    clearerr(stdin);
}

Boolean getUserInput(char * string, int maxstrlen){
	Boolean validInput = FALSE;
	while (validInput == FALSE){
		fgets(string, maxstrlen, stdin);
		/* No input entered */
		if (string[0] == '\n' || string[0] == '\0') 
			return FALSE;
		/* Too many characters entered */
		if (string[strlen(string)-1] != '\n'){
			readRestOfLine(); /* Clear buffer */
			printf("Too many characters entered.\nPlease try again:");
			continue;
		}
		validInput = TRUE;
	}
	if (string[strlen(string)-1] == '\n') 
		string[strlen(string)-1] = '\0';
	return TRUE;
}