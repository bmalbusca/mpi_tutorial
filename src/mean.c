/*
 * Let A be a large array of N integers. Write an MPI routine that efficiently calculate the mean value of the array and have process rank 3 print the result. Assume that A is initially only available to processor rank 7. For simplicity, you may assume that A can be evenly distributed among all the processes (i.e., N is divisible by the number of processors). Explicitly include in you code the data transfer routines (don’t worry about the exact MPI syntax, but be sure to specify all the relevant parameters).
 */

#include<stdio.h>
#include<mpi.h>
#define N 10




int main (int argc, char * argv[]){

    MPI_Init(NULL,NULL);
    
    int myrank, world_size;
    int arr[N], rarr[N]={0};

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    for(int i =0; i<N; ++i){
        arr[i]=i;
    }
    
    MPI_Barrier(MPI_COMM_WORLD);

    //if(myrank==7){
        MPI_Scatter(arr, N/world_size,MPI_INT, rarr, N/world_size, MPI_INT, myrank, MPI_COMM_WORLD); 
    //}
    
    
    MPI_Barrier(MPI_COMM_WORLD);

    printf("array from %i \n",myrank);
    for(int j=0; j<N; ++j){
    printf("(r=%i) %i\n", myrank, rarr[j]);
    }

    int local_sum=0, sum=0;
    for(int j=0;j<N/world_size;++j){
        local_sum += rarr[j];
    }


    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);

    if(myrank==0){
        printf("\n(rank=%i) Mean value =%f \n", myrank,(float)sum/(float)N);
    }


    
    
    

   MPI_Finalize();
    return 0;
}
 
