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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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

/**
 * count_tokens - Takes a shell command and returns the number of tokens
 *
 * (srt) The C string with the shell command
 * # Returns the number of tokens in the string
 */
int count_tokens(char* str)
{
	/* The current token count */
	int count;

	/* Status flag indicating if we are inside a token */
	int inside_word;

	/* An iterator for the string */
	char* it;

	count = 0;
	inside_word = 0;
	it = str;

	do
	{
		switch(*it)
		{
			case '\0':
			case ' ':
			case '\t':
			{
				if (inside_word)
				{
					inside_word = 0; count++;
				}
				break;
			default:
				inside_word = 1;
			}
		}
	}
	while(*it++);
	
	return count;
}


/**
 * handle_command - Executes the given command
 *
 * (command) The shell command to handle
 */
void handle_command(char* command)
{
	/* The var holding the pid of the child processes */
	pid_t pid;

	/* The status of the terminated child process */
	int status;

	/* The tokens count */
	int tok_count;

	tok_count = count_tokens(command);

	if (tok_count == 1)
	{
		pid = fork();
		if (pid == 0)
		{
			/* Code executed by child process */
			printf("Child: %s\n", command);
			/* Exit child process */
			exit(0);
		}
		else if (pid > 0)
		{
			/* Code executed by parent process */
			printf("Parent waiting for pid #%d\n", pid);
			wait(&status);
			printf("Child with pid #%d has returned with status %d\n", pid, status);
		}
		else
		{
			perror("fork");
		}
	}
	else if(tok_count > 1)
	{
		printf("Command line arguments not supported!\n");
	}
}

int main(int argc, char* argv[])
{
	/* Flag that indicates exit condition for the shell */
	char on_exit;

	/* The buffer that will hold the command given  */
	char command[512];

	/* The prompt string before the space that the user will enter his command */
	char* prompt_string = "CowShell>";

	/* The embedded exit command */
	char* exit_command = "exit";
	
	/* To avoid unused param warning */
	(void)(argc);
	(void)(argv);

	/* Set shell exit flag to false*/
	on_exit = 0;

	/* Main program loop */
	while (!on_exit)
	{
		/* Print the prompt string */
		printf("%s", prompt_string);
		
		/* Clear the command buffer */
		memset(command, 0, sizeof(command));
		
		if (fgets(command, sizeof(command), stdin) != 0)
		{
			/* Strip newline character */
			strip_newline(command);	
			
			/* Check if it is the embedded exit command */
			if (strcmp(command, exit_command) == 0)
			{
				on_exit = 1;
				break;	
			}
			else
			{
				/* Pass command to handler */
				handle_command(command);
			}
		}
		else
		{
			if (feof(stdin) != 0)
			{
				/* EOF has been set for stdin */
				on_exit = 1;
				printf("\nEOF received!\n");
			}
			else
			{
				perror("fgets");
			}
		}
	}

	return 0;
}
