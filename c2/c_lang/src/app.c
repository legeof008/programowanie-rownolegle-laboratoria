//
// Created by maciek on 3/15/23.
//
#include <matrix.h>
#include <thread_matrix_multiplier.h>

int main()
{
    matrix_t* l_matrix = self_new_from_file("A.txt");
    matrix_t* r_matrix = self_new_from_file("B.txt");

    matrix_t* result = multiply(l_matrix,r_matrix,3);
    self_print(result);
    self_free(l_matrix);
    self_free(r_matrix);
    return 0;
}