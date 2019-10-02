/*
Author: Mustapha Tidoo YUssif
*/

#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h>
#include <sys/wait.h>

int primality(int n);
int sumPrime(int n) ;
int sumEven(int m);
int parent();
int child();

int main(){

    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        fprintf(stderr, "child was not createdd");
        exit(EXIT_FAILURE);
        break;
    
    case 0:
        printf("\nChild <%d> belonging to parent <%d> is running\n", (int)getpid(), (int)getppid());
        child();
        break;

    default:
        wait(NULL);
        printf("\nParent<%d> with child <%d> is running\n", (int)getpid(), pid);
        parent();
        break;
    }

    return 0;
}

int parent(){
    printf("Suming prime numbers");
    printf("--------------------------------------");

    int input;
    printf("Enter an integer: ");
    scanf("%d", &input);

    int ans = sumPrime(input);
    printf("The sum of all prime numbers less than %d is %d\n\n", input, ans);

    return 0;
}

int child(){
    printf("Suming even numbers");
    printf("--------------------------------------");

    int input;
    printf("Enter an integer: ");
    scanf("%d", &input);

    int ans = sumEven(input);
    printf("The sum of all even numbers less than %d is %d\n\n", input, ans);

    return 0;
}

int sumPrime(int n){
    
    int i;
    int results = 0;
    for(i=2; i < n; i++){
        if(primality(i)==1){
            results += i;
        }
    }

    return results;
}


int primality(int n){

    //negative values and 0 are not prime
    if(n <=1){
        return 0;
    }

    //2 and 3 are prime
    if(n <= 3){
        return 1;
    }

    //numbers above 3 that are divisible by 2 or 3 are not prime. 
    if(n % 2 == 0 || n % 3 == 0){
        return 0;
    }

    //numbers divisible by numbers of the 6k + 5, (where
    //k=0,1,2,3,4....) are not prime.
    int i = 5;
    while(i*i < n){
        if (n%i==0 || n%(i+2)==0){
            return 0;
        }
        i = i + 6;
    }
    //number is prime is above conditions are met. 
    return 1;
       
}


int sumEven(int m){

    int j;
    int even = 0;

    for(j = 0; j < m; j++){
        if(j%2 == 0){
            even += j;
        }
    }
    return even;
}

