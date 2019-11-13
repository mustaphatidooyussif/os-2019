#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


typedef struct __lock_t{
	/*The flag of the lock*/
	int flag;
} lock_t;

lock_t mutex;
int balance;
int loops;

int TestAndSet(int *ptr, int new){
	//Enter code here
	int old = *ptr; 
	*ptr = new; 
	return old;
}

void init(lock_t *mutex){
	//Enter code here
	mutex->flag = 0;
}

void lock(lock_t *mutex){
	//Enter code here
	while(TestAndSet(&mutex->flag, 1)==1);
}

void unlock(lock_t *mutex){
	//Enter code here
	mutex->flag = 0;
}

void* run(){
	for(int i = 0; i < loops; i++){
		//Enter code here
		lock(&mutex);
		balance = balance + 1;
		//Enter code here
		unlock(&mutex);
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
