#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>

int find_primes(int rank_array[], int n ){
        
      int count=0,c=2;
    
      if(n<2){
        return 0;
      }
  
     for (int i = 2; i <= n;i++)
    {
        for (c = 2; c <= i - 1; c++)
        {
            if (i%c == 0){
                break;
            }
        }
        if (c == i)
        {
            rank_array[count]=i;  
            count++;
        }

    
  }

     return count;
}

int main (int argc, char** argv){

    MPI_Init(&argc, &argv);

    int rank, world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);


    MPI_Group world_group; //lets create a group reach the MPI world
    MPI_Comm_group(MPI_COMM_WORLD, &world_group); //accesses the group associated with given communicator 
    MPI_Group prime_group;
    //Now we will create a new sub-group which have only the prime ranks (world_group)
    int size_array_ranks, n = ((double)world_size/(double)log(world_size)); 
    int * ranks = (int*)calloc(n+1,sizeof(int));     
    
    size_array_ranks = find_primes(ranks,n-1);

    if (size_array_ranks > 0){
            MPI_Group_incl(world_group,size_array_ranks, ranks, &prime_group); 
            // Create a new communicator based on the group
            MPI_Comm prime_comm;
            MPI_Comm_create_group(MPI_COMM_WORLD, prime_group, 0, &prime_comm);

            int prime_rank = -1, prime_size = -1;
            // If this rank isn't in the new communicator, it will be MPI_COMM_NULL
            if (MPI_COMM_NULL != prime_comm) {
                MPI_Comm_rank(prime_comm, &prime_rank);
                MPI_Comm_size(prime_comm, &prime_size);
            }

            printf("WORLD RANK/SIZE: %d/%d --- PRIME RANK/SIZE: %d/%d\n",rank, world_size, prime_rank, prime_size);
            MPI_Group_free(&prime_group);

            if (MPI_COMM_NULL != prime_comm) {
                MPI_Comm_free(&prime_comm);
            }


    }
 
    /* //test function
    printf("rank=%i size=%i\n",rank,size_array_ranks);
    
    for(int j=0; j< size_array_ranks; ++j){
        printf("rank=%i prime=%i",rank,ranks[j] );
    }
    printf("rank=%i done\n",rank);
    */
    

    MPI_Group_free(&world_group);
  
    MPI_Finalize();


    return 0;
}