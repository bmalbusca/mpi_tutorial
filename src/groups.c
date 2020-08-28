#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<math.h>
/* Split communicator - MPI_Comm_split()
 *
 *  Odd/even splitting of processes:
    
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    color = rank%2;
    MPI_Comm_split (MPI_COMM_WORLD, color, rank, &newcomm); MPI_Comm_size (newcomm, &size);
    MPI_Comm_rank (newcomm, &rank);
 *
 *  Column-wise; Suppose your processors are in a two-dimensional grid - Create a communicator per column:
    
    MPI_Comm_rank( MPI_COMM_WORLD, &mytid );
    proc_i = mytid % proc_column_length; // row-wise selection    
    proc_j = mytid / proc_column_length; //column color
    MPI_Comm column_comm;
    MPI_Comm_split( MPI_COMM_WORLD, proc_j, mytid, &column_comm );
 *
 *
 * Split row-wise (4x4 matrix)
    
    int color = world_rank / 4; // Determine color based on row

    // Split the communicator based on the color and use the  original rank for ordering
    MPI_Comm row_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &row_comm);

    int row_rank, row_size;
    MPI_Comm_rank(row_comm, &row_rank);
    MPI_Comm_size(row_comm, &row_size);

    printf("WORLD RANK/SIZE: %d/%d \t ROW RANK/SIZE: %d/%d\n", world_rank, world_size, row_rank, row_size);
    MPI_Comm_free(&row_comm);

 * */

/* 
 * FUNTION: find_primes
 * ARGS: int *, int
 * DESCRIPTION:  Create a new sub-group which have only the prime ranks (world_group)
 *   int size_array_ranks, n = ((double)world_size/(double)log(world_size)); 
 *   int * ranks = (int*)calloc(n+1,sizeof(int));     
 *   size_array_ranks = find_primes(ranks,n-1);
 */
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
        {   rank_array[count]=i;  
            count++;
        }  
  }
     return count;
}


int find_oddeven(int array[], int nproc){
    
    int count=0;
    for(int i=0; i<nproc;i++){
        
        if(i%2!=0){
            array[i]=i;
            count++;
        }
    }

    return count ;
}


int main (int argc, char** argv){

    MPI_Init(&argc, &argv);

    int rank, world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);


    MPI_Group world_group; //lets create a group reach the MPI world
    MPI_Comm_group(MPI_COMM_WORLD, &world_group); //accesses the group associated with given communicator    
      
    //if we want to divide into odd ranks 
    int size_array_ranks, n = ((double)world_size/(double)2.0);
    int * ranks = (int*)calloc(n+1,sizeof(int));
    size_array_ranks = find_oddeven(ranks, world_size);
    
    MPI_Group prime_group;

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
 
    /* //test function for find_oddeven() 
    printf("rank=%i array_size=%i\n",rank,size_array_ranks);
    
    for(int j=0; j< size_array_ranks; ++j){
        printf("rank=%i prime=%i ",rank,ranks[j] );
    }
    printf("(rank=%i) done\n",rank);
    */
    

    MPI_Group_free(&world_group);
    MPI_Finalize();


    return 0;
}
