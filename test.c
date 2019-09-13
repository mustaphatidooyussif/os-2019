#include <stdio.h>
#include <stdlib.h>

int main(){

    FILE *myFile;
    myFile = fopen("matrixA.txt", "r");

    //read file into array
    int numberArray[16];
    int i;

    if (myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }

    for (i = 0; i < 16; i++){
        fscanf(myFile, "%d", &numberArray[i] );
    }

    for (i = 0; i < 16; i++){
        printf("Number is: %d\n\n", numberArray[i]);
    }

    fclose(myFile);


    //    int row = 2, col = 4;
    //     int *arr = (int *)malloc(row * col * sizeof(int)); 
    //     int i, j;
    //     for (i = 0; i < row; i++)
    //         for (j = 0; j < col; j++)
    //             *(arr + i*col + j) = i + j;  

    //     printf("The matrix elements are:\n");
    //     for (i = 0; i < row; i++) {
    //         for (j = 0; j < col; j++) {
    //             printf("%d ", *(arr + i*col + j)); 
    //         }
    //         printf("\n");
    //     }
    //     free(arr);

    return 0;

}