//
// Created by maciek on 3/14/23.
//
#ifndef C_LANG_MATRIX_H
#define C_LANG_MATRIX_H

#include <stdio.h>

#pragma pack(1)
typedef struct matrix_t {
    int num_of_columns, num_of_rows;
    double *matrix;
} matrix_t;


matrix_t *self_new(int num_of_columns, int num_of_rows);

matrix_t *self_new_from_file(char *source_filename);

void self_print(matrix_t *self);

void self_free(matrix_t *self);

#endif