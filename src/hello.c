#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc , char **argv){

    int world_size, rank;

    // Initialize the MPI environment
    MPI_Init(NULL,NULL); 

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    printf("Hello from core %i (%i/%i)\n", rank,rank,world_size);

    MPI_Finalize(); 

}
