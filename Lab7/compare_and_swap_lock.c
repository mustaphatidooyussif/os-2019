#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


typedef struct __lock_t{/*The flag of the lock*/} lock_t;

lock_t mutex;
int balance;
int loops;

int CompareAndSwap(int *ptr, int expected, int new){
	//Enter code here
}

void init(lock_t *mutex){
	//Enter code here
}

void lock(lock_t *mutex){
	//Enter code here
}

void unlock(lock_t *mutex){
	//Enter code here
}

void* run(){
	for(int i = 0; i < loops; i++){
		//Enter code here
		balance = balance + 1;
		//Enter code here
	}
}

//have a main function with 2 threads 
//that concurrently print the value of the flags.
int main(int argc, char * argv[]){
	init(&mutex);
	loops = atoi(argv[1]);
	pthread_t threads[2];


	for(int i = 0; i < 2; i++){
		pthread_create(&threads[i],NULL, run, NULL);
	}
	for(int i = 0; i < 2; i++){
		pthread_join(threads[i],NULL);
	}

	printf("The value of balance is: %d\n", balance);

	return 0;
}
