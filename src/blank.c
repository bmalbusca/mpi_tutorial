#include<stdio.h>
#include<mpi.h>


int main(int argc, char * argv[]){
    
    MPI_Init(NULL,NULL );
    // Initialize the MPI environment
    int rank,world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);









    MPI_Finalize();
    return 0;
}
