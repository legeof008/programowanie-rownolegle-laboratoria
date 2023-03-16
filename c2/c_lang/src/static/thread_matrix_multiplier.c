//
// Created by maciek on 3/15/23.
//
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include <thread_matrix_multiplier.h>

#define ARRAY_INDEX_DELTA 1;
#define NO_ATTRIBIUTES NULL
#define NO_STATUS_RETURN_POINTER NULL

matrix_t *multiplication_result;
matrix_t *g_left_matrix;
matrix_t *g_right_matrix;

int *g_thread_beginning_indexes;
int *g_thread_ending_indexes;
int **g_thread_column_row_tuple;
int g_index_increment;
double g_sum;
int matrix_len;


static inline void validate_matrices(matrix_t *left_matrix, matrix_t *right_matrix);

static inline void allocate_global_data_indicating_vectors(int num_of_threads);

static inline void populate_beginning_and_ending_indexes(int num_of_threads);

static inline void populate_touple_vector(int columns, int rows);

static inline void spawn_workers(pthread_t *threads_index, int num_of_workers);

static inline double multiply_row_to_column(int row, int column, matrix_t *left, matrix_t *right);

static inline int calculate_increment(int matrix_len, int num_of_threads);

static inline bool matrices_cannot_be_multiplied(matrix_t *left_matrix, matrix_t *right_matrix);

static inline void cleanup_global_index_vectors();

static inline void *multiply_appointed_rows_to_appointed_columns(void *worker_number) {
    int my_number = (int) worker_number;
    for (int i = g_thread_beginning_indexes[my_number]; i < g_thread_ending_indexes[my_number]; i++) {
        *(multiplication_result->matrix + i) =
                multiply_row_to_column(g_thread_column_row_tuple[i][0],
                                       g_thread_column_row_tuple[i][1],
                                       g_left_matrix,
                                       g_right_matrix);

        g_sum += *(multiplication_result->matrix + i);
#ifdef DEBUG
        printf("Thread %d counted - %lf\n", my_number, *(multiplication_result->matrix + i));
#endif
    }
    pthread_exit(NULL);
}

matrix_t *multiply(matrix_t *left_matrix, matrix_t *right_matrix, int num_of_threads) {

    g_sum = 0.;
    validate_matrices(left_matrix, right_matrix);
    g_left_matrix = left_matrix;
    g_right_matrix = right_matrix;

    multiplication_result = self_new(right_matrix->num_of_columns, left_matrix->num_of_rows);

    matrix_len = left_matrix->num_of_rows * right_matrix->num_of_columns;

    g_index_increment = calculate_increment(matrix_len, num_of_threads);

    allocate_global_data_indicating_vectors(num_of_threads);

    for (int i = 0; i < matrix_len; i++) {
        *(g_thread_column_row_tuple + i) = malloc(sizeof(int) * 2);
    }

    populate_touple_vector(multiplication_result->num_of_columns, multiplication_result->num_of_rows);

    populate_beginning_and_ending_indexes(num_of_threads);

    pthread_t *threads_index = malloc(sizeof(pthread_t *) * num_of_threads);
    spawn_workers(threads_index, num_of_threads);


    free(threads_index);
    cleanup_global_index_vectors();
    printf("Global sum is %lf\n", g_sum);
    return multiplication_result;
}

static inline void validate_matrices(matrix_t *left_matrix, matrix_t *right_matrix) {
    if (matrices_cannot_be_multiplied(left_matrix, right_matrix)) {
        perror("Error: matrices cannot be multiplied due to a column-to-row \n"
               "mismatch between them or either of them is NULL \n");
        exit(EXIT_FAILURE);
    }
}

static inline bool matrices_cannot_be_multiplied(matrix_t *left_matrix, matrix_t *right_matrix) {
    return left_matrix == NULL || right_matrix == NULL ||
           left_matrix->num_of_columns != right_matrix->num_of_rows;
}

static inline int calculate_increment(int matrix_len, int num_of_threads) {
    return (int) round((double) (matrix_len / num_of_threads));
}

static inline void allocate_global_data_indicating_vectors(int num_of_threads) {
    g_thread_ending_indexes = malloc(num_of_threads * sizeof(int));
    g_thread_beginning_indexes = malloc(num_of_threads * sizeof(int));
    g_thread_column_row_tuple = malloc(matrix_len * sizeof(int *));
}

static inline void populate_touple_vector(int columns, int rows) {

    int k = 0;
    for (int i = 0; i < multiplication_result->num_of_rows; i++) {
        for (int j = 0; j < multiplication_result->num_of_columns; j++) {
            if (k < 10) {
                **(g_thread_column_row_tuple + k) = i;
                *(*(g_thread_column_row_tuple + k) + 1) = j;
                k++;
            }
        }
    }
}

static inline void populate_beginning_and_ending_indexes(int num_of_threads) {
    g_thread_beginning_indexes[0] = 0;
    g_thread_ending_indexes[num_of_threads - 1] = matrix_len - ARRAY_INDEX_DELTA;

    for (int i = 1; i < num_of_threads; i++) {
        g_thread_beginning_indexes[i] = g_thread_beginning_indexes[i - 1] + g_index_increment;
        g_thread_ending_indexes[i - 1] = g_thread_beginning_indexes[i - 1] + g_index_increment - ARRAY_INDEX_DELTA;
    }
}

static inline void spawn_workers(pthread_t *threads_index, int num_of_workers) {
    for (int i = 0; i < num_of_workers; i++) {
        pthread_create(&threads_index[i], NO_ATTRIBIUTES, multiply_appointed_rows_to_appointed_columns, (void *) i);
    }
    for (int i = 0; i < num_of_workers; i++) {
        pthread_join(threads_index[i], NO_STATUS_RETURN_POINTER);
    }
}

static inline double multiply_row_to_column(int row, int column, matrix_t *left, matrix_t *right) {
    double result = 0.;
    for (int i = 0; i < left->num_of_columns; i++) {
        result += *(left->matrix + row + i) * *(right->matrix + column + i);
    }
    return result;
}

static inline void cleanup_global_index_vectors() {
    free(g_thread_ending_indexes);
    free(g_thread_beginning_indexes);
    for (int i = 0; i < matrix_len; i++) {
        free(*(g_thread_column_row_tuple + i));
    }
    free(g_thread_column_row_tuple);
}
