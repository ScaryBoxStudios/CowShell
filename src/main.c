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
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>


/**
 * struct TOKENDATA - A structure that stores the results of the splitstr function
 *
 * (tokens) A pointer used as a dynamically allocated array to dynamically allocated strings
 * (count) The number of dynamically allocated strings in the tokens array
 */
typedef struct TOKENDATA
{
	char** tokens;
	int count;
} TOKENDATA;


/**
 * strip_sur_whitespace - Removes the surrounding whitespace from a C String
 *
 * (str) the C string to process
 * # Returns 0 on success and -1 on error 
 */
int strip_sur_whitespace(char* str)
{
	/* An iterator */
	char* it;

	/* Holds the number of leading whitespace */
	int lead_wsp;

	/* if param invalid return error */
	if (!str)
		return -1;

	it = str;

	/* Set iterator to the end of the string */
	while(*it != '\0')
		it++;

	/* Strip trailing whitespace */
	it--;
	while(isspace(*it))
	{
		*it = '\0';
		it--;
	}
	
	/* Strip leading whitespace */
	it = str;
	lead_wsp = 0;
	while(isspace(*it))
	{
		lead_wsp++;
		it++;
	}

	if(lead_wsp != 0)
	{
		while(*it)
		{
			*(it - lead_wsp) = *it;
			it++;
		}
		*(it - lead_wsp) = '\0';
	}			
	
	return 0;
}


/**
 * tokcount - Takes a c string and some delimeters and counts the number of tokens the string will produce if we split it among them
 *
 * (str) The string to split
 * (delims) The delimeters
 * # Returns the number of tokens contained in the string or -1 if error occured
 */
int tokcount(char* str, char* delims)
{
	/* The total count */
	int count = 1;

	/* Temp buffer */
	char* buf;

	buf = malloc(sizeof(char) * (strlen(str) + 1));
	memcpy(buf, str, strlen(str) + 1);

	/* Return -1 on invalid params */
	if(!str || !delims)
		return -1;

	strtok(buf, delims);
	while(strtok(0, delims))
		count++;

	free(buf);

	return count;
}


/**
 * splitstr - Takes a c string and some delimeters and splits the string to them. Creates an array with the dynamically allocated substrings.
 *
 * (str) The string to split
 * (delims) The delimeters
 * # Returns a TOKENDATA dynamically allocated struct with the results
 */
TOKENDATA* splitstr(char* str, char* delims)
{
	/* Loop counter */
	int i = 0;

	/* The token data */
	TOKENDATA* td;

	/* Current token size */
	int tok_sz;

	/* Pointer to current strtok token */
	char* cur_token;

	td = malloc(sizeof(TOKENDATA));

	/* Get count of all tokens */
	td->count = tokcount(str, delims);

	/* Allocate the array of C strings */
	td->tokens = malloc(sizeof(char*) * (td->count + 1));

	/* Get first token */
	cur_token = strtok(str, delims);
	
	/* Store first token */
	tok_sz = strlen(cur_token);
	(td->tokens)[0] = malloc(tok_sz + 1);
	memcpy((td->tokens)[0], cur_token, tok_sz + 1);

	/* Get remaining tokens */
	for(i = 1; i < td->count; i++)
	{
		cur_token = strtok(0, delims);
		tok_sz = strlen(cur_token);
		(td->tokens)[i] = malloc(tok_sz + 1);
		memcpy((td->tokens)[i], cur_token, tok_sz + 1);
	}
	
	/* Fill the after last pos with null to be compatitible with exec */
	(td->tokens)[i] = 0;
	
	return td;
}


/**
 * destroy_tokendata - Deallocates memory allocated by a splitstr call
 *
 * (data) A TOKENDATA pointer pointing to the structure to deallocate
 */
void destroy_tokendata(TOKENDATA* data)
{
	int i;
	for(i = 0; i < data->count; i++)
		free(data->tokens[i]);
	free(data->tokens);
	free(data);
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

	/* The tokens of the current command */
	TOKENDATA* tok_dt;

	pid = fork();
	if (pid == 0)
	{
		/* Code executed by child process */
		tok_dt = splitstr(command, " \t");
#ifdef _DEBUG
		printf("Child executing command: %s\n", command);
#endif
		if (execvp(command, tok_dt->tokens) == -1)
		{
			/* If exec returns (on error) we need to deallocate the resources, because the current process is not thrown away */
			destroy_tokendata(tok_dt);		
	
			/* Handle the exec error */
			perror("exec");
			exit(-1);
		}
	}
	else if (pid > 0)
	{
		/* Code executed by parent process */

#ifdef _DEBUG
		printf("Parent waiting for pid #%d\n", pid);
#endif
		wait(&status);
#ifdef _DEBUG
		printf("Child with pid #%d has returned with status %d\n", pid, status);
#endif
	}
	else
	{
		perror("fork");
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
			/* Strip surrounding whitespace */
			strip_sur_whitespace(command);	
			
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
