#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INTERVAL 10000

int main() {
    int circle_points = 0;
    int square_points = 0;
    srand(time(NULL));

#pragma omp parallel for shared(circle_points,square_points)
    for (int i = 0; i < INTERVAL * INTERVAL; i++) {
        int rand_x = (double) (rand() % (INTERVAL + 1) / INTERVAL);
        int rand_y = (double) (rand() % (INTERVAL + 1) / INTERVAL);
        int origin_dest = rand_x * rand_x + rand_y * rand_y;

        if (origin_dest < 1) {
#pragma omp critical
            circle_points++;
            square_points++;
        } else {
#pragma omp critical
            square_points++;
        }
    }

    double pi = (double) circle_points * 4 / (double) square_points;

    printf("Number of circle points  = %d, Number of square points = %d\n, Pi is %lf\n", circle_points, square_points, pi);
    return 0;
}
