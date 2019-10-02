/*
Author: David Sasu
Purpose of the program: The purpose of the program is to:
1. Create a parent process and a child process
2. Transfer the string value 'Greetings' from the parent process to the child process. 
3. Make the child process read the string value transfered from the parent and print it out on the screen.  
*/

/* Pre-processor directives*/
#include <sys/types.h> // We need to include it for the type definitions of our functions
#include <stdio.h> // There would be some input and output happening in this program
#include <stdlib.h> //There would be the usage of some functions, whose definition can be found in the standard library
#include <unistd.h> //There would be the usage of some constants in the program
#include <string.h> //There would be string manipulations in this program

/* Defining the constants that would be used in the program

Reason:
The reason why we are defining constants is that their values are not going to change through out the course of 
the program. The constants that we are going to be defining are the read and write values of the pipe and the buffer
size of the pipe.
*/
#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void){
	//The main program creates the message that is to be sent to the child process
	char write_message[BUFFER_SIZE] = "Greetings";
	char read_message[BUFFER_SIZE];

	//create the array that is to function as a pipe
	int pipe_channel[2];
	pid_t pid;

	//create the pipe
	if(pipe(pipe_channel) == -1){
		fprintf(stderr,"The pipe could not be created");
		return 1;
	}

	//If the pipe could be created, create the child process
	pid = fork();

	if(pid < 0){ //The child could not be created 
		fprintf(stderr, "The child could not be created!");
		return 1;
	}

	if(pid > 0){ //The parent process 
		/*:
		1. Close the read-end of the pipe that the parent is not using
		2. Write the message into the write-end of the pipe for the child
		3. Close the write-end of the pipe after the message has been written
		*/
		FILE *fRead = fopen("read.txt", "r");
		if(fRead == NULL){
			fprintf(stderr, "File not found\n");
		}

		
		close(pipe_channel[READ_END]);
		write(pipe_channel[WRITE_END], write_message, strlen(write_message)+1);
		close(pipe_channel[WRITE_END]);
	}

	else{ //The child process
		/*
		1. Close the part of the pipe that the child would not use, which is the write part of the pipe.
		2. Read the information from the parent from the read part of the pipe.
		3. Close the read part of the pipe.
		*/
		close(pipe_channel[WRITE_END]);
		read(pipe_channel[READ_END],read_message,BUFFER_SIZE);
		printf("read: %s\n",read_message);
		close(pipe_channel[READ_END]);
	}

	return 0;
}


