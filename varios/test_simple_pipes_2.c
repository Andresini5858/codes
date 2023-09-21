/*
* Copyright (C) ViGIR Vision-Guided and Intelligent Robotics Lab
* Written by Guilherme DeSouza, Luis Rivera
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, meaning:
* keep this copyright notice,
* do not try to make money out of it,
* it's distributed WITHOUT ANY WARRANTY,
* yada yada yada...
*/

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function called by the child process.
// Inputs: file descriptors associated to the pipes
void child(int pipe_p, int pipe_c)
{
	char string_p[] = "Hello parent!";	// message to be sent to the parent
	char readbuffer[80];

	while(1)
	{
		sleep(1);

		if(write(pipe_p, string_p, (strlen(string_p)+1)) != (strlen(string_p)+1))
		{
			printf("child's pipe write error\n");
			exit(-1);
		}

		if(read(pipe_c, readbuffer, sizeof(readbuffer)) < 0)
		{
			printf("child's pipe read error\n");
			exit(-1);
		}

		printf("child: %s\n", readbuffer);
	}
}

int main(void)
{
	int pid;
	int pipe_p[2], pipe_c[2];	// size has to be 2

	char string_c[] = "Hello child!";	// message to be sent to the child
	char readbuffer[80];

	if(pipe(pipe_p) < 0)
	{
		printf("pipe creation error\n");
		exit(-1);
	}

	if(pipe(pipe_c) < 0)
	{
		printf("pipe creation error\n");
		exit(-1);
	}

	if((pid = fork()) < 0)
	{
		printf("fork error\n");
		exit(-1);
	}

	if(pid == 0)  // child process
	{
		close(pipe_p[0]);	// not really needed, but it emphasizes that these sides
		close(pipe_c[1]);	// are not needed for the child process (in this example)
		child(pipe_p[1], pipe_c[0]);
	}

	// parent process
	close(pipe_p[1]);		// not really needed, but it emphasizes that these sides
	close(pipe_c[0]);		// are not needed for the parent process (in this example)

	while(1)
	{
		if(read(pipe_p[0], readbuffer, sizeof(readbuffer)) < 0)
		{
			printf("parent's pipe read error\n");
			exit(-1);
		}
		printf("parent: %s\n", readbuffer);

		sleep(1);
		if(write(pipe_c[1], string_c, (strlen(string_c)+1)) != (strlen(string_c)+1))
		{
			printf("parent's pipe write error\n");
			exit(-1);
		}
	}

	return 0;
}
