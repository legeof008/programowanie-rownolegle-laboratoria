//
// Created by maciek on 3/5/23.
//
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <math.h>

#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "sum.h"
#include "try.h"

#define READ_WRITE_FLAG 0644
#define DECIMAL_BASE 10
#define NO_ADDR 0
#define EMPTY_FLAG 0
#define FROM_BEGINNING 0
#define ALL_PIDS -1
#define NO_BUFF NULL
#define FTOK_PRIME_1 111
#define FTOK_PRIME_2 1011
#define FTOK_PRIME_3 303
#define FILE_READ_MODE "r"
#define SELF_FILENAME "sum.c"

sigset_t global_process_mask;
struct sigaction global_usr1_action;
double * global_input_data_vector;
double * global_result_vector;
int* global_index_vector;
int global_index_increment;

struct sum_vector_t *self_new(char *filename) {
    FILE *file_stream;
    try
    {
        file_stream = fopen(filename, FILE_READ_MODE);
        throw_if_null(file_stream);
    }
    catch(...) {
    perror("Error: File could not be opened\n");
    exit(EXIT_FAILURE);
}

    char file_read_buffer[FILE_READING_BUFFER_SIZE + NULL_TERMINATED_CHARACTER_OFFSET];
    int number_of_vector_elements;

    fgets(file_read_buffer, FILE_READING_BUFFER_SIZE, file_stream);
    number_of_vector_elements = (int) strtol(file_read_buffer, NULL, DECIMAL_BASE);

    sum_vector_t *new_sum_vector = malloc(sizeof(sum_vector_t));
    new_sum_vector->length = number_of_vector_elements;
    new_sum_vector->data = malloc(number_of_vector_elements * sizeof(double));

    for (int i = 0; i < number_of_vector_elements; i++) {
        fgets(file_read_buffer, FILE_READING_BUFFER_SIZE, file_stream);
        new_sum_vector->data[i] = strtod(file_read_buffer, NULL);
    }
    fclose(file_stream);

    return new_sum_vector;

}

void self_free(sum_vector_t * self) {
    free(self->data);
    free(self);
}

void self_print_elements(sum_vector_t *self) {
    int num_of_elements = self->length;
    for (int i = 0; i < num_of_elements; i++) {
        printf("%f ", self->data[i]);
    }
    printf("\n");
}

void on_usr1(int signal) {
    printf("USR1 recived ...\n");
    if(global_input_data_vector == NULL)
        printf("nullisimo\n");
    exit(EXIT_SUCCESS);
}
void on_usr2(int signal) {
    printf("Force quit pid %d\n",getpid());
    exit(EXIT_FAILURE);
}

void configure_signal_handling_and_await() {
    sigemptyset(&global_process_mask);

    global_usr1_action.sa_handler = (&on_usr1);
    global_usr1_action.sa_mask = global_process_mask;
    global_usr1_action.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &global_usr1_action, NULL);

    sigemptyset(&global_process_mask);

    global_usr1_action.sa_handler = (&on_usr2);
    global_usr1_action.sa_mask = global_process_mask;
    global_usr1_action.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR2, &global_usr1_action, NULL);

    pause();
}


void spawn_processes(pid_t *subprocess_index, int number_of_processes) {
    for (int i = 0; i < number_of_processes; i++) {
        try
        {
            subprocess_index[i] = fork();
            throw_if_fail(subprocess_index[i]);
        }
        catch(...) {
        perror("Error: Child process could not be created\n");
        exit(EXIT_FAILURE);
        }
        if (subprocess_index[i] == Child) {
            configure_signal_handling_and_await();
        }
    }
}

int calculate_index_base(int number_of_processes, int vector_length) {
    return (int) round((double) vector_length / number_of_processes);
}

void send_to_all(pid_t* subprocess_index, int number_of_processes, int signal){
    for(int i = 0; i < number_of_processes;i++)
    {
        kill(subprocess_index[i], signal);
    }
}

void populate_sm_vector(double* sh_vector, sum_vector_t* source){
    for(int i = 0; i < source->length; i++)
    {
        sh_vector[i] = source->data[i];
    }
}

void populate_shared_memory_indexes(int* sh_indexes_vector, int increment, int vector_len){
    for(int i = 0; i < vector_len; i++)
    {
        sh_indexes_vector[i] = i * increment;
        //printf("I%d = %d\n",i,sh_indexes_vector[i]);
    }
}

void cleanup(pid_t* subprocess_index,int number_of_processes,char* cleanup_message){
    printf("%s\n",cleanup_message);
    send_to_all(subprocess_index,number_of_processes,SIGUSR2);
    free(subprocess_index);
}

int main(int argc, char **argv) {

    int number_of_processes = 3;

    pid_t *subprocess_index = malloc(number_of_processes * sizeof(pid_t));
    spawn_processes(subprocess_index, number_of_processes);

    sum_vector_t *v = self_new("vector.dat");

    if (number_of_processes > v->length) {
        cleanup(subprocess_index,number_of_processes,"Too many tasks for processes !");
        exit(EXIT_FAILURE);
    }

    int shared_memory_data_id, shared_memory_results_id, shared_memory_start_indexes_id;
    key_t shared_memory_data_key = ftok(SELF_FILENAME,FTOK_PRIME_1);
    key_t shared_memory_result_key = ftok(SELF_FILENAME,FTOK_PRIME_2);
    key_t shared_memory_start_indexes_key = ftok(SELF_FILENAME,FTOK_PRIME_3);
    try
    {
        shared_memory_data_id = shmget(shared_memory_data_key, v->length * sizeof (double ), READ_WRITE_FLAG | IPC_CREAT);
        throw_if_fail(shared_memory_data_id);
        shared_memory_results_id = shmget(shared_memory_result_key, number_of_processes, READ_WRITE_FLAG | IPC_CREAT);
        throw_if_fail(shared_memory_results_id);
        shared_memory_start_indexes_id = shmget(shared_memory_start_indexes_key, number_of_processes, READ_WRITE_FLAG | IPC_CREAT);
        throw_if_fail(shared_memory_start_indexes_id);
    }
    catch(...) {
        perror("Error: Could not initialise shared memory.\n");
        exit(EXIT_FAILURE);
    }

    global_input_data_vector = shmat(shared_memory_data_id, NO_ADDR, EMPTY_FLAG);
    global_result_vector = shmat(shared_memory_results_id, NO_ADDR, EMPTY_FLAG);
    populate_sm_vector(global_input_data_vector, v);

    global_index_increment = calculate_index_base(number_of_processes,v->length);
    printf("Global index increment is %d\n",global_index_increment);
    global_index_vector = shmat(shared_memory_start_indexes_id, NO_ADDR, EMPTY_FLAG);
    populate_shared_memory_indexes(global_index_vector,global_index_increment,number_of_processes);

    kill(subprocess_index[0], SIGUSR1);
    waitpid(ALL_PIDS, FROM_BEGINNING, EMPTY_FLAG);

    // shared memory cleanup and normal cleanup
    printf("Destroying global shared memory input data vector... Exit status  %d\n",shmctl(shared_memory_data_id,IPC_RMID,NO_BUFF));
    printf("Destroying global shared memory results vector... Exit status %d\n",shmctl(shared_memory_results_id,IPC_RMID,NO_BUFF));
    printf("Destroying global shared memory indexing data vector... Exit status %d\n",shmctl(shared_memory_start_indexes_id,IPC_RMID,NO_BUFF));
    printf("Vec_size = %d\n", v->length);
    self_print_elements(v);
    cleanup(subprocess_index,number_of_processes,"The program ended successfully.\n");
    self_free(v);
}
