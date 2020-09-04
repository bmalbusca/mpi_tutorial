#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>



int main(int argc , char  ** argv){


    MPI_Init(NULL,NULL);

    //Split communicator - MPI_Comm_split() to Odd/even splitting of processes:
    int myrank, world_size, error;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank (MPI_COMM_WORLD, &myrank);

    //. For example, if you were to define color = rank%2; (with rank the rank of the process in comm), then you would create (globally) two new communicators: one for the processes of odd ranks, and one for the processes of even ranks. However, keep in mind that each processes will only be seeing the one of these new communicators they are part of... So in summary, color permits to tell apart the various "teams" you will create, like the colour of the jersey football teams will wear to distinguish themselves during a match (hence the naming I presume).
    int color = myrank%2;
    int new_rank, new_size;
    MPI_Comm evencomm; 

    error = MPI_Comm_split (MPI_COMM_WORLD, color, myrank, &evencomm);
    //Tasks with the same color end-up in the same communication group (evecommm).
    MPI_Comm_size (evencomm, &new_size);
    MPI_Comm_rank (evencomm, &new_rank);

    printf("%i WORLD RANK/SIZE: %d/%d \t ROW RANK/SIZE: %d/%d COLOR GROUP:%i\n",error, myrank, world_size, new_rank, new_size, color);

    if ( evencomm != MPI_COMM_NULL ) {
        MPI_Barrier(evencomm); 
        MPI_Comm_free(&evencomm);
    }
    
    MPI_Barrier(MPI_COMM_WORLD); 
    if(myrank==0){
        printf("EVEN COMMUNICATORS  - DONE\n\n");
    }
 
    
    //Column-wise; Suppose your processors are in a two-dimensional grid - Create a communicator per column:

    int  proc_column_length = world_size/2;
    int col_rank, col_size, color2;
    float  proc_i = myrank % proc_column_length; // column-wise selection
    float  proc_j = myrank / proc_column_length; //row-wise color
    color2 = proc_i;

    MPI_Comm column_comm;
    error = MPI_Comm_split( MPI_COMM_WORLD, proc_i, myrank, &column_comm );
    MPI_Comm_size (column_comm, &col_size);
    MPI_Comm_rank (column_comm, &col_rank);

    printf("%i WORLD RANK/SIZE: %d/%d \t COLUMN RANK/SIZE: %d/%d COLOR GROUP:%i\n",error, myrank, world_size, new_rank, new_size, color2);

     if ( column_comm != MPI_COMM_NULL ) {
         MPI_Barrier(column_comm);
         MPI_Comm_free(&column_comm); 
    }
    
     MPI_Barrier(MPI_COMM_WORLD); 
    if(myrank==0){
        printf("COLUMN COMMUNICATORS  - DONE\n\n");
    }
    
    //Split row-wise (nxn matrix) - if row length is 4, we can use color=rank/4
    int  color3 = myrank %  world_size; // Determine color based on row Split the communicator based on the color and use the  original rank for orderin
    
    MPI_Comm row_comm;
    int row_rank, row_size;

    error = MPI_Comm_split(MPI_COMM_WORLD, color3, myrank, &row_comm);    
    MPI_Comm_rank(row_comm, &row_rank);
    MPI_Comm_size(row_comm, &row_size);
    
    printf("%iWORLD RANK/SIZE: %d/%d \t ROW RANK/SIZE: %d/%d COLOR GROUP:%i\n",error, myrank, world_size, row_rank, row_size, color3);
   
     if ( row_comm != MPI_COMM_NULL ) {
         MPI_Barrier(row_comm);
         MPI_Comm_free(&row_comm);
    }

     MPI_Barrier(MPI_COMM_WORLD);
    if(myrank==0){
        printf("ROW COMMUNICATORS  - DONE\n\n");
    }

 

MPI_Finalize();
return 0; 

}
