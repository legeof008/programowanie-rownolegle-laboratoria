//
// Created by Maciej Michalski on 20/04/2023.
//
#include<mpi.h>
#include<stdio.h>

/* A two-dimensional torus of 12 processes in a 4x3 grid */
int main(int argc, char *argv[]) {
    int vector [1000000];

    int rank, size;
    MPI_Comm comm;
    int dim[2], period[2], reorder;
    int coord[2], id;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    MPI_Request request;


    dim[0] = 4;
    dim[1] = 1;
    period[0] = 1;
    period[1] = 1;
    reorder = 1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm);

    if(rank == 0)
    {

        int dest_rank;
        MPI_Cart_shift(comm, 0, 1, &rank, &dest_rank);
        int sub_vector [1000];
        for(int i = 0; i < 1000; i++)
        {
            sub_vector[i%1000] = vector[i];
            if(i%1000 == 0)
            {
                MPI_Isend(sub_vector,1000,MPI_INT,dest_rank,0,comm,&request);
                MPI_Wait(&request,&status);

            }
        }
    }

    //recieve
    if(rank != 0) {
        int new_dest;
        MPI_Cart_shift(comm, 0, 1, &rank, &new_dest);
        int sub_vec_self[1000];
        for(int i = 0; i < 1000; i++)
        {
            MPI_Irecv( sub_vec_self, 1000, MPI_INT,MPI_ANY_SOURCE , 0, comm, &request);
            MPI_Wait(&request,&status);
            MPI_Isend(sub_vec_self,1000,MPI_INT,new_dest,0,comm,&request);
            MPI_Wait(&request,&status);
            printf("Rank %d got message, sending to %d\n",rank,new_dest);
        }

    }
    MPI_Finalize();
    return 0;
}