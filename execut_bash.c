#include <stdio.h>
#include <stdlib.h> 
#include <sys/wait.h> 
#include <unistd.h>

int main(){
    int n;

    printf("EXAM EXAM EXAM");

    if(fork() == 0){
        execlp(".mybash.sh", "mybash.sh", "echo", (char *) NULL);
    }
    else{
        wait(&n);
    }
}

// int fd[2];
// pipe(fd);
// if ( (childpid = fork() ) == -1){
//    fprintf(stderr, "FORK failed");
//    return 1;
// } else if( childpid == 0) {
//    close(1);
//    dup2(fd[1], 1);
//    close(fd[0]);
//    execlp("/bin/sh","/bin/sh","-c",parameters,NULL);
// }
// wait(NULL);
// read(fd[0], result, RESULT_SIZE);
// printf("%s\n",result);