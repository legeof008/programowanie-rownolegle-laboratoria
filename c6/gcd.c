#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// Function to calculate the greatest common divisor (GCD) of two numbers
int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

void calculate(int num,int rank,int size) {
    printf("Process %d - Value: %d\n", rank, num);

    for (int i = 1; i < size; i *= 2) {
        int partner = (rank + i)%size;
        //printf("i:%d rank:%d partner:%d num:%d\n",i,rank,partner, num);
        int recv_num;
       
        // Send local value to partner process
        MPI_Send(&num, 1, MPI_INT, partner, 0, MPI_COMM_WORLD);

        // Receive value from partner process
        MPI_Recv(&recv_num, 1, MPI_INT, (size+(rank-i))%size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("i:%d rank:%d partner:%d wyslane_num:%d otrzymane_num:%d\n", i, rank, partner, num, recv_num);
        // Update local value to be reduced in the next iteration
        num = gcd(num, recv_num);
        
        printf("i:%d rank:%d partner:%d obliczone num:%d\n", i, rank, partner, num);
    }

    if (rank == 0) {
        printf("The greatest common divisor (NWD) is: %d\n", num);
    }
}

int main(int argc, char* argv[]) {
    int rank, size;
    //int nwd = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //int num[] = { 252,324,396,432,648,900,1260,1620 };
    //int num[] = {21000, 24000, 30000, 42000, 48000, 54000, 60000, 66000};
    //int num[] = { 214578, 465432, 897654, 120987, 987654, 543210, 765432 };
    /*int num[] = {
    112589424, 22250498, 33252598, 44258600, 5525600, 662589602, 77250602, 88252604, 992592604, 11078606, 12196604, 1328608, 1898424, 1599410, 188420, 1762612
    };*/
    int num[] = {36,72,96,144,108,216,216,252};
    //for (int i = 0;i < 16;i++)
     //   num[i] = num[i] % 1000000;
    // Initialize local value for each process
    calculate(num[rank], rank, size);

    MPI_Finalize();
    return 0;
}
