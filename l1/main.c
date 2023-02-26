//
// Created by maciek on 2/26/23.
//
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#include "dice.h"

#define CHILD_PROCESS 0
#define CHILD_PROCESS_FAIL -1
int main()
{

    dice* self = self_new(1,6,3);
    int i = self->num_of_results - 1;
    pid_t *subprocess = malloc(i);
    int r;
    for (; i > 0; i--) {
        subprocess[i] = fork();
        switch (subprocess[i]) {
            case CHILD_PROCESS:
                srandom(time(NULL) ^ getpid());
                r = ((int) random() % 6) +1;
                self->results[i] = r;
                printf("Subprocess random num is %d for index i = %d\n",r,i);
                return EXIT_SUCCESS;
                break;
            case CHILD_PROCESS_FAIL:
                return EXIT_FAILURE;
                break;
            default:
                wait(0);
                break;
        }
    }
    srandom(time(NULL) ^ getpid());
    printf("Subprocess random num is %d for index i = %d\n",((int) random() % 6) +1,0);
    return 0;
}