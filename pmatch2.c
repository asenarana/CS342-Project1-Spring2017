#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

// function signature
void childProcess();

// global variables
char keyword[64];
char inputFile[64];
int pipes[20][2];
int childNo;

// ze main
int main(int argc, char *argv[])
{
	FILE *output;
	FILE *intermediate;
	int status;
	int inputCount;
	pid_t pid;
	char line[262];
	pid_t children[20];

	// read inputs
	strcpy(keyword, argv[1]);
	inputCount = atoi(argv[2]);
	for(int a = 0; a < inputCount; a++)
	{
		strcpy(inputFile, argv[a+3]);
		childNo = a;
		pipe(pipes[a]);
		// fork
		pid = fork();
		if( pid < 0)
		{
			// fork failed
			exit(0);
			
		}
		else if( pid == 0)
		{
			// child process
			childProcess();
			return 0;
		}
		else
		{
			// parent process
			close(pipes[a][1]);
			children[a] = pid;
		}
	}
	// wait for children to die
	for(int b = 0; b < inputCount; b++)
	{
		do
		{
			waitpid(children[b], &status, WNOHANG);
		}
		while(!WIFEXITED(status));
	}
	// pipe to output file
	output = fopen(argv[argc-1], "w");
	for( int c = 0; c < inputCount; c++)
	{
		intermediate = fdopen(pipes[c][0], "r");
		if( intermediate != NULL)
		{
			while(!feof(intermediate))
			{
				fputs(argv[c+3], output);
				fputs(", ", output);
				fgets(line, 262, intermediate);
				fputs(line, output);
			}
			fclose(intermediate);
		}
	}
	// clean the mess
	fclose(output);
	exit(0);
}

void childProcess()
{
	FILE *input;
	FILE *output;
	char line[256];
	char lineTok[256];
	char *token;
	int lineNo = 1;
	const char space[2] = " ";
	
	close(pipes[childNo][0]);
	input = fopen(inputFile, "r");
	output = fdopen(pipes[childNo][1], "w");
	if( input != NULL)
	{
		while(!feof(input))
		{
			// BRUTE FORCE!! search for key
			fgets(line, 256, input);
			strcpy(lineTok, line);
			token = strtok(lineTok, space);
			while(token != NULL)
			{
				if(strcmp(token, keyword) == 0)
				{
					// write into file
					fprintf(output, "%d: ", lineNo);
					fputs(line, output);
					break;
				}
				token = strtok(NULL, space);
			}
			lineNo++;
		}
		fclose(input);
	}
	fclose(output);
	return;
}

