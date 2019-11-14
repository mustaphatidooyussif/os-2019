#include <stdio.h> 
#include <stdlib.h> 


void unEfficientSort(int *arr, int size){
    //sorting the burst time in increasing order. 
      for (int c = 0 ; c < size - 1; c++)
        {
            for (int d = 0 ; d < size - c - 1; d++)
            {
            if (arr[d] > arr[d+1]) 
            {
                int swap       = arr[d];
                arr[d]   = arr[d+1];
                arr[d+1] = swap;
            }
            }
        }

    return;
}

int main(int argc, char * argv[]){

    //initialization of variables. 
    int num_process = 0; 
    int wait_time_sum = 0;
    int turnaround_sum = 0;
    int *burst_times, *waiting_times, *turnaround_times;

    //take inputs from user
    printf("Enter the no. of process: "); 
    scanf("%d", &num_process); 

    //allocate memory 
    burst_times = (int *) calloc(num_process, sizeof(int));
    waiting_times = (int *) calloc(num_process, sizeof(int));
    turnaround_times = (int *) calloc(num_process, sizeof(int));

   
    for (int i=0; i < num_process; i ++){
        printf("Enter the burst time of P%d: ", i+1);
        scanf("%d", &burst_times[i]);
    }

    unEfficientSort(burst_times, num_process);

    waiting_times[0] = 0;
    turnaround_times[0] = burst_times[0];
    for (int j=1; j< num_process; j++){
        waiting_times[j] = waiting_times[j-1] + burst_times[j-1];  //waiting time
        turnaround_times[j] = turnaround_times[j-1] + burst_times[j]; //turnaround time
    }

    //output results in tabular form
    
    printf("process id \t bt \t wt \t tt \n");
    for (int k= 0; k < num_process; k++){
        printf("\t %d \t %d \t %d \t %d \n", k + 1, burst_times[k], waiting_times[k], turnaround_times[k]);
        
        //Add up wait times and turnaround times. 
        wait_time_sum = wait_time_sum + waiting_times[k]; 
        turnaround_sum = turnaround_sum + turnaround_times[k];
    }

    //calcullate average wait time and average turnaround time for all processes. 
    printf("Average wait time = %d \n", wait_time_sum/num_process);
    printf("Average turnaround time = %d \n", turnaround_sum/num_process);

    free(burst_times);free(waiting_times);free(turnaround_times);

    return 0; 
}