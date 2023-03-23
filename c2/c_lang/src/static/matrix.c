//
// Created by maciek on 3/14/23.
//
#include <stdlib.h>

#include "static/try.h"
#include "static/matrix.h"

#define FILE_READ_MODE "r"
#define FILE_READING_BUFFER_SIZE 65536
#define NULL_TERMINATED_CHARACTER_OFFSET 1
#define DECIMAL_BASE 10
#define NO_END_POINTER NULL

matrix_t *self_new(int num_of_columns, int num_of_rows) {

    matrix_t *new = malloc(sizeof(matrix_t));

    new->num_of_columns = num_of_columns;
    new->num_of_rows = num_of_rows;
    new->matrix = malloc(sizeof(double) * num_of_rows * num_of_columns);

    try
    {
        throw_if_null(new);
        throw_if_null(new->matrix);
    }
    catch(...) {
    perror("Error: could not allocate memory for a new vector \n");
    exit(EXIT_FAILURE);
}

    return new;
}

matrix_t *self_new_from_file(char *source_filename) {
    FILE *file_stream;
    try
    {
        file_stream = fopen(source_filename, FILE_READ_MODE);
        throw_if_null(file_stream);
    }
    catch(...) {
    perror("Error: could not open a source file to read matrix from \n");
    exit(EXIT_FAILURE);
}
    char file_read_buffer[FILE_READING_BUFFER_SIZE + NULL_TERMINATED_CHARACTER_OFFSET];
    int number_of_rows, number_of_columns;

    fgets(file_read_buffer, FILE_READING_BUFFER_SIZE, file_stream);
    number_of_rows = (int) strtol(file_read_buffer, NO_END_POINTER, DECIMAL_BASE);

    fgets(file_read_buffer, FILE_READING_BUFFER_SIZE, file_stream);
    number_of_columns = (int) strtol(file_read_buffer, NO_END_POINTER, DECIMAL_BASE);


    matrix_t *new = self_new(number_of_columns, number_of_rows);

    double temporary_val;

    for (int row_i = 0; row_i < number_of_rows; row_i++) {
        for (int col_j = 0; col_j < number_of_columns; col_j++) {

            fscanf(file_stream, "%s", file_read_buffer);
            temporary_val = strtod(file_read_buffer, NO_END_POINTER);
            *(new->matrix + (row_i * number_of_columns) + col_j) = temporary_val;
        }
    }

    fclose(file_stream);
    return new;
}

void self_print(matrix_t *self) {
    for (int i = 0; i < self->num_of_rows; i++) {
        printf("[ ");
        for (int j = 0; j < self->num_of_columns; j++) {
            printf("%lf ",*((self->matrix + i * self->num_of_columns) + j));
        }
        printf("]\n");
    }
}

void self_free(matrix_t *self) {
    free(self->matrix);
    free(self);
}

