/*
Author: Mustapha Tidoo YUssif
*/

#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(){

    //create fork

    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        fprintf(stderr, "child was not createdd");
        exit(EXIT_FAILURE);
        break;
    
    case 0:
        printf("\nChild <%d> belonging to parent <%d> is running\n", (int)getpid(), (int)getppid());
        char filename[50];
        char str [] = "./";
        
        printf("Enter the name of the execuatable <e.g program > : ");
        scanf("%s", filename);

        strcat(str, filename);

        printf("------%s",  filename);

        char *args[]={str, NULL};  //  ./filename

        execvp(args[0],args); 
    
        break;

    default:
        wait(NULL);
        printf("\nParent<%d> with child <%d> is running\n", (int)getpid(), pid);
        char wcFile[50];
        
        printf("Enter the name of the file to count the words from: ");
        scanf("%s", wcFile);

        char *parArgs[4];
        parArgs[0] = strdup("wc");
        parArgs[1] = strdup(wcFile);
        parArgs[3] =  NULL; 
        execvp(parArgs[0], parArgs);
        break;
    }

    return 0;
}