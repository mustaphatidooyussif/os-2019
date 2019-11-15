#include <stdio.h>
#include <stdlib.h> 

#define ARRAYS_SIZE  20

int main(int argc, char * argv[]){

    //initialization of variables. 
    int num_process = 0; 
    int sum_wait_time = 0;
    int sum_tt_time = 0;
    int *bt, *wt, *tt;

    //take inputs from user
    printf("Enter the no. of process: "); 
    scanf("%d", &num_process); 

    //allocate memory 
    bt = (int *) calloc(num_process, sizeof(int));
    wt = (int *) calloc(num_process, sizeof(int));
    tt = (int *) calloc(num_process, sizeof(int));

    if (bt == NULL || wt == NULL || tt == NULL){
        fprintf(stderr, "Memory not allocated \n");
        exit(1);
    }
    
    for (int i=0; i < num_process; i ++){
        printf("Enter the burst time: ");
        scanf("%d", &bt[i]);
    }

    //initialize the wait time of process 1 to 0
    wt[0] = 0; 

    //initialize complettion time of process 1 to its burst time
    tt[0] = bt[0];

    //calculate the wait time of the rest
    for(int j=1; j<num_process; j++){
        wt[j] = wt[j-1] + bt[j-1]; //wait times
        tt[j] = tt[j-1] + bt[j]; //turnaround time
    }

    //output results in tabular form
    
    printf("process id \t bt \t wt \t tt \n");
    for (int k= 0; k < num_process; k++){
        printf("\t %d \t %d \t %d \t %d \n", k + 1, bt[k], wt[k], tt[k]);
        
        //Add up wait times and turnaround times. 
        sum_wait_time = sum_wait_time + wt[k]; 
        sum_tt_time = sum_tt_time + tt[k];
    }

    //calcullate average wait time and average turnaround time for all processes. 
    printf("Average wait time = %d \n", sum_wait_time/num_process);
    printf("Average turnaround time = %d \n", sum_tt_time/num_process);

    free(bt); free(wt); free(tt);
    return 0;
}