#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>
#include <time.h>

#define N 10000000
#define S (int)sqrt(N)
#define M N / 10

int main(int argc, char **argv)
{

    long int a[S + 1];    /*tablica pomocnicza*/
    long int pierwsze[M]; /*liczby pierwsze w przedziale 2..N*/
    long i, k, liczba, reszta;
    long int lpodz;      /* l. podzielnikow*/
    long int llpier = 0; /*l. liczb pierwszych w tablicy pierwsze*/
    double czas;         /*zmienna do  mierzenia czasu*/
    FILE *fp;

    clock_t t;
    t = clock();
/*wyznaczanie podzielnikow z przedzialow 2..S*/
    #pragma omp parallel for
    for (i = 2; i <= S; i++)
        a[i] = 1; /*inicjowanie*/
    for (i = 2; i <= S; i++)
        if (a[i] == 1)
        {
            pierwsze[llpier++] = i; /*zapamietanie podzielnika*/
            /*wykreslanie liczb zlozonych bedacych wielokrotnosciami i*/
            #pragma omp parallel for
            for (k = i + i; k <= S; k += i)
                a[k] = 0;
        }

    lpodz = llpier; /*zapamietanie liczby podzielnikow*/
    /*wyznaczanie liczb pierwszych*/
    printf("Llpier: %ld\n", llpier);
    #pragma omp parallel for shared(llpier, pierwsze) private(reszta, k) //num_threads(4)
    for (liczba = S + 1; liczba <= N; liczba++)
    {
        bool reszta_is_zero = false;
        #pragma omp parallel default(none) shared(liczba, pierwsze, reszta_is_zero, lpodz) private(reszta) //num_threads(4)
        {
            #pragma omp for
            for (k = 0; k < lpodz; k++)
            {
                reszta = (liczba % pierwsze[k]);
                if (reszta == 0)
                {
                    /*liczba zlozona*/
                    #pragma omp critical
                    {
                        reszta_is_zero = true;
                    }
                    #pragma omp cancel for
                }
            }
        }
        if (!reszta_is_zero)
        {
            #pragma omp critical
            {
                pierwsze[llpier++] = liczba; /*zapamietanie liczby pierwszej*/
            }
        }
    }
    printf("Llpier: %ld\n", llpier);

    if ((fp = fopen("primes.txt", "w")) == NULL)
    {
        printf("Nie moge otworzyc pliku do zapisu\n");
        exit(1);
    }
    for (i = 0; i < llpier; i++)
        fprintf(fp, "%ld ", pierwsze[i]);
    fclose(fp);
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("Time: %f\n", time_taken);
    return 0;
}