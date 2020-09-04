#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include <math.h>
/*  +=============================
    | 0(0,0)  |  1(0,1) | 2(0,2)  |
    ==============================
    | 3(1,0)  |  4(1,1) | 5(1,2)  |
    ==============================
    | 6(2,0   |  7(2,1) | 8(2,2)  |
    ==============================

    In this case we have 8 processors in a 3x3 matrix distribution: Cartesian Grid 3x3
    So, for the example, we will have 3 column groups(3 colors) and 3 row groups. 
*/
int main(int argc, char ** argv){

    MPI_Init(&argc, &argv);
    
    int myrank,world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); 

    // We wil split the World into 2 groups: columns and rows
    // Lets assume a square nxn matrix for the processors distribution
    // So, assuming that we can find CPU with 1,2,4,8,16, etc processors, we need to have a scalable solution 
    
    
    int n = log(world_size) / log(2);
    int color_row = myrank/n; //row with size n
    int color_col = myrank%n;
    int err1,err2;
    
    MPI_Comm colcomm, rowcomm;

    err1 = MPI_Comm_split(MPI_COMM_WORLD,color_row,color_col,&rowcomm );
    err2 = MPI_Comm_split(MPI_COMM_WORLD,color_col,color_row,&colcomm );
    
    int rank_col,rank_row;
    int size_col,size_row;

    MPI_Comm_rank(rowcomm,&rank_row );
    MPI_Comm_size(rowcomm,&size_row);

    MPI_Comm_rank(colcomm,&rank_col);
    MPI_Comm_size(colcomm,&size_col);


    printf("PROCESS %i (%i,%i); ROW GROUP=%i; COL GROUP=%i;\n", myrank,rank_row,rank_col, color_row,color_col);

    //but we can do the same using MPI_CART_CREATE()
    int ndims=2;     // 2D
    int dim[2]={n,n} ;// Proc by row, roc by cols
    int periods[2] = {0,0}; // no periodic boundaries - array that specifies the periodicity in each dimension
    
    // if periods[0]=1, which means that any reference beyond the first or last entry of any column will be wrapped around cyclically - row periodic (each column forms a ring)
    
    int reorder=0; // no reordering of processes 
    MPI_Comm cartcomm;

    MPI_Cart_create(MPI_COMM_WORLD,ndims,dim,periods,reorder,&cartcomm);
   

    MPI_Finalize();

    return 0;
}
