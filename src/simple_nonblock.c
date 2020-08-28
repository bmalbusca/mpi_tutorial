#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(argc,argv) int argc; char *argv[];
{
    int numtasks, rank, dest, source, rc, count, tag=1;  
    char inmsg, outmsg='x';
    MPI_Status stats[2];

    MPI_Request req[2]; 

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        dest = 1; 
        source = 1;
        rc = MPI_Irecv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &req[0]);
        rc = MPI_Isend(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &req[1]);
    } else if (rank == 1) {
        dest = 0;
        source = 0;
        rc = MPI_Irecv(&inmsg, 1, MPI_CHAR, source, tag, MPI_COMM_WORLD, &req[0]);
        rc = MPI_Isend(&outmsg, 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD, &req[1]);
    }
   MPI_Waitall (2 , req , stats );

    rc = MPI_Get_count(&stats[0], MPI_CHAR, &count);
    printf("Task %d: Received %d char(s) from task %d with tag %d \n",       rank, count, stats[0].MPI_SOURCE, stats[0].MPI_TAG);
    MPI_Finalize();
}
