//
// Created by maciek on 3/15/23.
//

#ifndef C_LANG_THREAD_MATRIX_MULTIPLIER_H
#define C_LANG_THREAD_MATRIX_MULTIPLIER_H

#include <matrix.h>

/*
 * Multiplies two matrixes as per usual mathematical method.
 * The order of the matrixes makes a diffrence.
 * @return matrix resulting from multiplication
 */
matrix_t* multiply(matrix_t* left_matrix, matrix_t* right_matrix, int num_of_threads);

#endif //C_LANG_THREAD_MATRIX_MULTIPLIER_H
