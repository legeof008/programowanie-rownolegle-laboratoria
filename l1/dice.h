//
// Created by maciek on 2/26/23.
//

#ifndef PROGRAMOWANIE_ROWNOLEGLE_LABORATORIA_DICE_H
#define PROGRAMOWANIE_ROWNOLEGLE_LABORATORIA_DICE_H

typedef struct dice {
    int range_max;
    int range_min;
    int num_of_results;
    int* results;
}dice;

dice* self_new(int range_min, int range_max, int num_of_results);

#endif //PROGRAMOWANIE_ROWNOLEGLE_LABORATORIA_DICE_H
