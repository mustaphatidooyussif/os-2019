/*
Mustapha Tidoo Yussif
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


/*
Singly linked list implementation
*/
typedef struct __node_t{
    int val;
    struct __node_t *next;
} node_t;

node_t *start = NULL; //head of the singly linked list
int list_size = 0;

/*
insert() adds the value at the head(first)
*/
void insert(int value){
    //Allocate memory equal to node size
    node_t *t;

    t = malloc(sizeof(node_t));

    if (t == NULL){
        fprintf(stderr, "Cannot allocate memory");
        exit(1);
    }
    
    t->val = value;   //set value of node
    t->next = start;  //set next of node
    start = t; 
    list_size =  list_size + 1;
    return;
}

void printLinkedList(){

    if (start == NULL){
        return;
    }

    while (start->next != NULL){
        printf(" %d -> ", start->val);
        start = start->next; 
    }
    printf(" %d \n", start->val);
}

void freeMemory(){
    if(start == NULL){
        return;
    }

    while (start !=NULL){
        node_t * temp = start;         
        start = start->next;
        free(temp);
    }
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
        insert(list_size);
		//Enter code here
		unlock(&mutex);
	}
    return NULL;
}

//have a main function with 2 threads 
//that concurrently print the value of the flags.
int main(int argc, char * argv[]){

    /*Should take 1 arguements <program> <loop_count>*/
    if(argc !=2){
        fprintf(stderr, "Usage: <program> <loop_count> \n");
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

	printf("The value of list_size is: %d\n", list_size);

    printf("The list is: "); 
    printLinkedList();

    freeMemory();

	return 0;
}
