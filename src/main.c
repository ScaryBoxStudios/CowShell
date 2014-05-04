/**********************************************************************************************************************/
/*               ______             ______                                                                            */
/*              /     /\           /     /\                                                                           */
/*             /     /  \         /     /  \                                                                          */
/*            /     / /\ \       /     / /\ \                                                                         */
/*           /_____/ /  \ \     /     / /  \ \                                                                        */
/*           \     \ \___\/    /     /  \___\ |                                                                       */
/*         ___\_    \ \       /     / /\ \  / /                                                                       */
/*        /_____/\___\ \     /_____/ /  \ \/ /                                                                        */
/*        \     \ \  / /     \     \ \___\  /                                                                         */
/*         \     \ \/ /       \     \ \  / /                                                                          */
/*          \     \  /         \     \ \/ /                                                                           */
/*           \_____\/           \_____\__/                                                                            */
/*                                                                                                                    */
/*                                                                                                                    */
/*      (C) 2014, ScaryBox Studios. All rights reserved.                                                              */
/*      Licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International Public License  */
/*      For license description see:                                                                                  */
/*               http://creativecommons.org/licenses/by-nc-nd/4.0/legalcode                                           */
/*               or read LICENCE file.                                                                                */
/*                                                                                                                    */
/*      Authors: Agorgianitis Loukas <agorg_louk@icloud.com>                                                          */
/*               Vlahakis Dimitris   <HeroPP7@gmail.com>                                                              */
/*                                                                                                                    */
/**********************************************************************************************************************/

#include <stdio.h>
#include <string.h>

/**
 * strip_newline - Removes newline character if exists from the end of a C string
 * 
 * (str) The C string to remove the newline character
 * # Returns 0 on success and 1 on error 
 */
int strip_newline(char* str)
{
	size_t length;
	
	/* Get the original length of the string */
	length = strlen(str);
	
	length = length - 1;

	/* Check if the string has a new line in the end */
	if (str[length] != '\n')
		return -1;

	str[length] = '\0'; 
	
	return 0;	
}

int main(int argc, char* argv[])
{
	/* The buffer that will hold the command given  */
	char command[512];

	/* The embedded exit command */
	char* exit_command = "exit";

	/* The prompt string before the space that the user will enter his command */
	char* prompt_string = "CowShell>";

	/* To avoid unused param warning */
	(void)(argc);
	(void)(argv);

	/* Main program loop */
	do 
	{
		/* Print the prompt string */
		printf("%s", prompt_string);
		
		/* Clear the command buffer */
		memset(command, 0, sizeof(command));
		
		if (fgets(command, sizeof(command), stdin) != 0)
		{
			/* Strip newline character */
			strip_newline(command);	

			/* DUMMY print back command */
			printf("%s\n", command);
		}
		else
		{
			perror("fgets");
		}
	}
	while (strcmp(command, exit_command) != 0);

	return 0;
}
