/*
Author: Mustapha Tidoo Yussif
Date: 10/09/2019 
Assigment: Lab 2

This is a simple program that reads two matrice from two files, 
multiply them and write the results back into another file. 
 */

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#define MAX_ROW_A 4;
#define MAX_COL_A 2;


int main(){
    int matA[4][2];
    FILE *f1 = fopen("matrixA.txt", "r");
    char c[10];

    if(f1 == NULL){
        printf("Cannot open file\n");
        exit(1);
    }
    int j=0;
    while(!feof(f1)){
        fscanf(f1, "%d", &matA[j]);
        j++;
    }

    for (int i = 0; i < 2; i++){
        printf("Number is: %d\n\n", matA[i]);
    }

    return 0;
}
