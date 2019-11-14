#include <stdio.h> 
#include <stdlib.h> 


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
    bt = (int *) malloc(num_process*sizeof(int));
    wt = (int *) malloc(num_process*sizeof(int));
    tt = (int *) malloc(num_process*sizeof(int));

   
    for (int i=0; i < num_process; i ++){
        printf("Enter the burst time: ");
        scanf("%d", &bt[i]);
    }


    //sorting the burst time in increasing order. 
      for (int c = 0 ; c < num_process - 1; c++)
        {
            for (int d = 0 ; d < num_process - c - 1; d++)
            {
            if (bt[d] > bt[d+1]) 
            {
                int swap       = bt[d];
                bt[d]   = bt[d+1];
                bt[d+1] = swap;
            }
            }
        }

    wt[0] = 0;
    tt[0] = bt[0];
    for (int j=1; j< num_process; j++){
        wt[j] = wt[j-1] + bt[j-1];  //waiting time
        tt[j] = tt[j-1] + bt[j]; //turnaround time
    }

    
    printf("process id \t bt \t wt \t tt \n");
    for (int k= 0; k < num_process; k++){
        printf("\t %d \t %d \t %d \t %d \n", k + 1, bt[k], wt[k], tt[k]);
        
        sum_wait_time = sum_wait_time + wt[k]; 
        sum_tt_time = sum_tt_time + tt[k];
    }

    printf("Average wait time = %d \n", sum_wait_time/num_process);
    printf("Average turnaround time = %d \n", sum_tt_time/num_process);

    free(bt);
    free(wt);
    free(tt);

    return 0; 
}