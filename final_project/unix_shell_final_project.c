#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(){
    char *buffer; 
    size_t buffer_size = 32; 
    size_t characters; 

    buffer = (char *) malloc(buffer_size * sizeof(char));

    if( buffer == NULL){
        perror("Memory error");
        exit(1);
    }

    printf("TYpe something");
    characters = getline(&buffer, &buffer_size, stdin);
    printf("%zu ", characters);
    printf("%s ", buffer);

    return 0;
}