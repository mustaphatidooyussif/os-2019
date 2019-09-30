#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h> 
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char * argv []){

    if(argc !=3){
        fprintf(stderr, "Usage: <ordinary_pipes_lab4> <type> <side>");
        exit(1);
    }

    if(atoi(argv[1]) < 3){
        fprintf(stderr, "Number of size cannot be less than 3"); 
        exit(1);
    }

    if(atof(argv[2]) < 1){
        fprintf(stderr, "Cannot have 0 or negative dimension");
        exit(1);
    } 

    int fd[2]; // file descriptors  
    pid_t pid; 
    int type = atoi(argv[1]);
    double size = atof(argv[2]);

    if(pipe(fd) == -1){
        fprintf(stderr, "Failed to create pipe"); 
        exit(1);
    }

    pid = fork();

    if(pid < 0){
        fprintf(stderr, "Fork failed");
        exit(1); 
    }

    if(pid > 0){
        //parent process 

        //close the read end 
        close(fd[READ_END]);

        /*Area of a regular polygon is n x 1/2 x s x a. Where n=number of sides, 
        s= dimension of the sides, a = apothem = s/2tan(180/n)*/
        double area = (size * size * type) / (4 * tan(3.14159/type));
        printf("The area of the polygon is %lf\n square unit", area);

        write(fd[WRITE_END], &type, sizeof(type)); //write type
        write(fd[WRITE_END], &size, sizeof(size));  //write dimension

        //close the write end
        close(fd[WRITE_END]);

        //Wait for children to finish
        wait(NULL);
    }else{
        //child process 
        
        //close the write end 
        close(fd[WRITE_END]);

        int type = 0;
        int size = 0; 
        read(fd[READ_END], &type, sizeof(argv[1]));  //read type 
        read(fd[READ_END], &size, sizeof(argv[2])); //read size

        int triangle = type - 2; 
        printf("Number of traingles contained in the polygon is %d\n", triangle);
        close(fd[READ_END]);
    }
    return 0;
}