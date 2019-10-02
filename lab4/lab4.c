/*
Author: Mustapha Tidoo Yussif
*/

#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h> 
#include <string.h>

#define READ_END 0
#define WRITE_END 1
#define PI 3.14159

double areaPolygon(int type, double dimension);

int main(int argc, char * argv []){

    /*Should take two arguements <type> <dimension>*/
    if(argc !=3){
        fprintf(stderr, "Usage: <lab4> <type> <dimension>\n");
        exit(EXIT_FAILURE);
    }

    /*Polygons have 3 or more sides*/
    if(atoi(argv[1]) < 3){
        fprintf(stderr, "oooops, no such polygon type\n");
        exit(EXIT_FAILURE);
    }

    /*Dimension of side cannot be zero or negative*/
    if(atof(argv[2]) < 1){
        fprintf(stderr, "ooooops, Wrong polygon dimension\n"); 
        exit(EXIT_FAILURE);
    } 

    int fd[2]; //file descriptot

    if(pipe(fd) == -1){
        fprintf(stderr, "Pipe failed"); 
        exit(EXIT_FAILURE);
    }

    
    switch(fork()){
        case -1: 
          fprintf(stderr, "Fork failed");
          break; 
        case 0: /*child process*/
          //close the write end 
          close(fd[WRITE_END]);

          int n; 
          int s;
          read(fd[READ_END], &n, sizeof(n));  //read type 
          read(fd[READ_END], &s, sizeof(s)); //read size

          int num_traingle = n - 2; 
          printf("The polygon has %d triangles\n", num_traingle);
          close(fd[READ_END]);

          break; 
        default: /*parent process*/
          //close the read end of the pipe
          close(fd[READ_END]);

          //area of polygon
          int type = atoi(argv[1]);
          double dim = atof(argv[2]);
          double area = areaPolygon(type, dim);
          printf("The area of the polygon with %d sides is %.2lf\n", type, area);

          //Write type 
          write(fd[WRITE_END], &type, sizeof(type)); 
          write(fd[WRITE_END], &dim, sizeof(dim));  
        
          //close the write end
          close(fd[WRITE_END]);

          //Wait for children to finish
          wait(NULL);
          break;
    }
    return 0;
}

double areaPolygon(int type, double dimension){
    /*The area of a regular polygon can be found by, 1/2 x apothem x dimension
    apotherm = dimension/2tan(180/dimension. Thus, the area becomes 
    (dimension x dimension x type)/4 x tan(180/dimension)
    */
    double num = (dimension * dimension * type);
    double denum = 4 * tan(PI/type);
    double area = num/ denum; 

    return area;
}