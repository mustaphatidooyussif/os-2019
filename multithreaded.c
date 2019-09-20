#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 

volatile int counter = 0;
int loop;

void *worker(void *str);

int main(int argc, char * argv[]){

    if (argc !=2){
        fprintf(stderr, "Usage: filename <value>");
        exit(1);
    }

    loop = atoi(argv[1]);

    pthread_t p1, p2;

    printf("Iniitial value of counter: %d\n", counter);

    pthread_create(&p1, NULL, worker, NULL);
    pthread_create(&p2, NULL, worker, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("Final value of counter: %d\n", counter);

    return 0;

}

void *worker(void *str){

    int i;

    for(i =0; i < loop; i++){
        counter ++;
    }

    return NULL;
}