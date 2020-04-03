/******************************************************************************
** Student name: 	Sherman Wong
** Student number: 	s3656264
** Course: 			Advanced Programming Techniques - S1 2018
******************************************************************************/

#include "vm.h"

int main(int argc, char ** argv)
{
	VmSystem * system = malloc(sizeof(VmSystem));
	MenuItem menu[MENU_SIZE];
	MenuFunction choice;
	if(systemInit(system) == FALSE){
		printf("Error - System initialisation failed.\n");
        return EXIT_FAILURE;
	}
    if (loadData(system, argv[1], argv[2]) == TRUE){
        initMenu(menu);
        do {
			choice = getMenuChoice(menu);
			printf("\n");
			if (choice == NULL)
				continue;
			(*choice)(system);	
		} while (choice != menu[2].function && choice != menu[8].function);
	} 
	else
		abortProgram(system);
	printf("Goodbye! \n\n");
	return EXIT_SUCCESS;
}