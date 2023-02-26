//
// Created by maciek on 2/26/23.
//
#include <stdlib.h>
#include "dice.h"

dice *self_new(int range_min, int range_max, int num_of_results) {
    dice *new_dice = malloc(sizeof(dice));
    new_dice->results = malloc(num_of_results);
    new_dice->num_of_results = num_of_results;
    new_dice->range_max = range_max;
    new_dice->range_min = range_min;
    return new_dice;
}




