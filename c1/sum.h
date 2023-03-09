//
// Created by maciek on 3/5/23.
//

#ifndef PROGRAMOWANIE_ROWNOLEGLE_LABORATORIA_SUM_H
#define PROGRAMOWANIE_ROWNOLEGLE_LABORATORIA_SUM_H

#define FILE_READING_BUFFER_SIZE 80
#define NULL_TERMINATED_CHARACTER_OFFSET 1

typedef struct sum_vector_t {
    int length;
    double *data;
} sum_vector_t;

typedef enum process_status{
    Failed = -1,
    Child = 0
} process_status;

struct sum_vector_t *self_new(char *filename);

#endif //PROGRAMOWANIE_ROWNOLEGLE_LABORATORIA_SUM_H
