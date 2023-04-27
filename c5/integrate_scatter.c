//
// Created by maciek on 4/4/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

typedef double (*integrated_function_t)(double);

double g_result = 0;
double gg_res = 0.;
double gg_inc = 0.;

void
integrate(integrated_function_t function, double begin_idx, double ending_idx, int num_of_points, int process_rank,
          int size_of_cluster) {


    int idx_len = num_of_points / size_of_cluster;

    double rectangle_len = (ending_idx - begin_idx) / num_of_points;
    double starting_indexes[num_of_points];
    double scattered_data[idx_len];
    for (int i = 0; i < num_of_points; i++) {
        starting_indexes[i] = begin_idx + i * rectangle_len;
        //printf("idx = %f\n", starting_indexes[i]);
    }
    MPI_Scatter(&starting_indexes, idx_len, MPI_DOUBLE, &scattered_data, idx_len, MPI_DOUBLE, 0,
                MPI_COMM_WORLD);

    printf("Process has received: %f %f \n", scattered_data[0], scattered_data[1]);
    for (int i = 1; i <= idx_len; i++) {

        double result_local = (rectangle_len) * function(scattered_data[i]);
        printf("local_result = %f\n", result_local);
        g_result += result_local;
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Gather(&g_result, 1, MPI_DOUBLE, &gg_inc, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    g_result += gg_inc;
    MPI_Finalize();
}

int main(int argc, char **argv) {
    int process_rank, size_of_cluster;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    integrate(&sin, 1, 10, 100, process_rank, size_of_cluster);


    printf("g_result = %f\n", g_result);
    return 0;
}