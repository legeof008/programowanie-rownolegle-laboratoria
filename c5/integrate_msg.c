//
// Created by maciek on 4/4/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

typedef double (*integrated_function_t)(double);
#define SENDER 0
double g_result = 0;

void
integrate(integrated_function_t function, double begin_idx, double ending_idx, int num_of_points, int process_rank,
          int size_of_cluster) {

    int idx_len = num_of_points / size_of_cluster;

    double rectangle_len = (ending_idx - begin_idx) / num_of_points;
    double starting_indexes[num_of_points];
    double scattered_data[idx_len];
    MPI_Request request;
    for (int i = 0; i < num_of_points; i++) {
        starting_indexes[i] = begin_idx + i * rectangle_len;
        printf("idx = %f\n", starting_indexes[i]);
    }
    if (process_rank == SENDER) {
        for (int i = 0; i <= size_of_cluster - 1; i++) {
            for (int j = 0; j < idx_len; j++) {
                scattered_data[j] = starting_indexes[i * size_of_cluster + j];
            }
            MPI_Send(&scattered_data, idx_len, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&scattered_data, idx_len, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Message Received: %f\n", scattered_data[0]);

    }
    for (int i = 1; i <= idx_len; i++) {
        //double result_local = (scattered_data[i] + i * rectangle_len) * function(begin_idx);
        //g_result += result_local;
        g_result++;
        MPI_Barrier(MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv) {
    int process_rank, size_of_cluster;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    integrate(NULL, 1, 10, 100, process_rank, size_of_cluster);

    MPI_Finalize();
    printf("g_result = %f\n", g_result);
    return 0;
}