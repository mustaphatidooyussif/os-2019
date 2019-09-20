/*
Author: Mustapha Tidoo YUssif
*/

#include <stdlib.h>
#include <stdio.h> 


void transferData(char *f1, char *f2);

int main(int argc, char * argv[]){

    char file1[50], file2[50];
    
    printf("Enter the source file name e.g source.txt: ");
    scanf("%s", file1);

    printf("\nEnter the target file name e.g target.txt: ");
    scanf("%s", file2);

    //call function
    transferData(file1, file2);

    return 0;
}

void transferData(char *f1, char *f2){
    printf("\n Am going to transfer data from source to target");
    //attempt to open the source file with read only permision
    FILE *s = fopen(f1, "r");
    
    //If failed
    if( s == NULL){
        fprintf(stderr, "No such file or directory called %s", f1);
        exit(EXIT_FAILURE);
    }

    //attempt to open the target file with write only permision
    FILE *t = fopen(f2, "w");
        //If failed
    if( t == NULL){
        fprintf(stderr, "No such file or directory called %s", f2);
        fclose(s);
        exit(EXIT_FAILURE);
    }

    while(!feof(s)){
        char c = fgetc(s);
        fputc(c, t);
    }

    printf("\n Finished copying......\n");

}