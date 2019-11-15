#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


//NOde struct
typedef struct __Node{
    int val;
    struct __Node *next;
} Node;

Node *start = NULL; //head of the singly linked list
int size = 0;

/*
addToFirst() method adds to beginning of list.
*/
void addToFirst(int value){
    //Allocate memory equal to node size
    Node *n = malloc(sizeof(Node));
    
    n->val = value;   //set value of node
    n->next = start;  //set next of node
    start = n; 
    size =  size + 1;
    }

void print_list(){

    if (start == NULL){
        return;
    }

    while (start->next != NULL){
        printf(" %d ", start->val);
        start = start->next; 
    }
    printf(" %d \n", start->val);
}

typedef struct __lock_t{
	/*The flag of the lock*/
	int flag;
} lock_t;

lock_t mutex;
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
        addToFirst(size);
		//Enter code here
		unlock(&mutex);
	}
	return NULL;
}

//have a main function with 2 threads 
//that concurrently print the value of the flags.
int main(int argc, char * argv[]){

    if(argc !=2){
        fprintf(stderr, "Usage: <file> <loop> \n");
        exit(EXIT_FAILURE);
    }

	init(&mutex);
	loops = atoi(argv[1]);
	pthread_t threads[2];


	for(int i = 0; i < 2; i++){
		pthread_create(&threads[i],NULL, run, NULL);
	}
	for(int i = 0; i < 2; i++){
		pthread_join(threads[i],NULL);
	}

	printf("The value of size is: %d\n", size);

    print_list();

    //release momery
    if(start != NULL){
        while (start !=NULL){
            Node * temp = start;         
            start = start->next;
            free(temp);
        }
    }

	return 0;
}
